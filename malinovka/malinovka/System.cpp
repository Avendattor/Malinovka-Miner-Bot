#include "System.h"
#include <Iphlpapi.h>
#include <memory>

#pragma comment(lib, "iphlpapi.lib")

char* System::GetMacAddress()
{
	PIP_ADAPTER_INFO AdapterInfo;
	DWORD dwBufLen = sizeof(IP_ADAPTER_INFO);
	char* mac_addr = (char*)malloc(18);

	AdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
	if (AdapterInfo == NULL) {
		printf("Error allocating memory needed to call GetAdaptersinfo\n");
		free(mac_addr);
		return NULL; // it is safe to call free(NULL)
	}

	// Make an initial call to GetAdaptersInfo to get the necessary size into the dwBufLen variable
	if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW) {
		free(AdapterInfo);
		AdapterInfo = (IP_ADAPTER_INFO*)malloc(dwBufLen);
		if (AdapterInfo == NULL) {
			printf("Error allocating memory needed to call GetAdaptersinfo\n");
			free(mac_addr);
			return NULL;
		}
	}

	if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR) {
		// Contains pointer to current adapter info
		PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
		do {
			// technically should look at pAdapterInfo->AddressLength
			//   and not assume it is 6.
			sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
				pAdapterInfo->Address[0], pAdapterInfo->Address[1],
				pAdapterInfo->Address[2], pAdapterInfo->Address[3],
				pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
			//	printf("Address: %s, mac: %s\n", pAdapterInfo->IpAddressList.IpAddress.String, mac_addr);
				// print them all, return the last one.
				// return mac_addr;

				//printf("\n");
			pAdapterInfo = pAdapterInfo->Next;
		} while (pAdapterInfo);
	}
	free(AdapterInfo);
	return mac_addr; // caller must free.
}

std::string System::GetFirstSerialNumber()
{
	//get a handle to the first physical drive
	HANDLE h = CreateFileW(L"\\\\.\\PhysicalDrive0", 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (h == INVALID_HANDLE_VALUE) return {};
	//an std::unique_ptr is used to perform cleanup automatically when returning (i.e. to avoid code duplication)
	std::unique_ptr<std::remove_pointer<HANDLE>::type, void(*)(HANDLE)> hDevice{ h, [](HANDLE handle) {CloseHandle(handle); } };
	//initialize a STORAGE_PROPERTY_QUERY data structure (to be used as input to DeviceIoControl)
	STORAGE_PROPERTY_QUERY storagePropertyQuery{};
	storagePropertyQuery.PropertyId = StorageDeviceProperty;
	storagePropertyQuery.QueryType = PropertyStandardQuery;
	//initialize a STORAGE_DESCRIPTOR_HEADER data structure (to be used as output from DeviceIoControl)
	STORAGE_DESCRIPTOR_HEADER storageDescriptorHeader{};
	//the next call to DeviceIoControl retrieves necessary size (in order to allocate a suitable buffer)
	//call DeviceIoControl and return an empty std::string on failure
	DWORD dwBytesReturned = 0;
	if (!DeviceIoControl(hDevice.get(), IOCTL_STORAGE_QUERY_PROPERTY, &storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY),
		&storageDescriptorHeader, sizeof(STORAGE_DESCRIPTOR_HEADER), &dwBytesReturned, NULL))
		return {};
	//allocate a suitable buffer
	const DWORD dwOutBufferSize = storageDescriptorHeader.Size;
	std::unique_ptr<BYTE[]> pOutBuffer{ new BYTE[dwOutBufferSize]{} };
	//call DeviceIoControl with the allocated buffer
	if (!DeviceIoControl(hDevice.get(), IOCTL_STORAGE_QUERY_PROPERTY, &storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY),
		pOutBuffer.get(), dwOutBufferSize, &dwBytesReturned, NULL))
		return {};
	//read and return the serial number out of the output buffer
	STORAGE_DEVICE_DESCRIPTOR* pDeviceDescriptor = reinterpret_cast<STORAGE_DEVICE_DESCRIPTOR*>(pOutBuffer.get());
	const DWORD dwSerialNumberOffset = pDeviceDescriptor->SerialNumberOffset;
	if (dwSerialNumberOffset == 0) return {};
	const char* serialNumber = reinterpret_cast<const char*>(pOutBuffer.get() + dwSerialNumberOffset);
	return serialNumber;
	return std::string();
}
