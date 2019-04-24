#pragma once

#include <string>
#include <stdint.h>
#include <windows.h>
#include <appmodel.h>


std::wstring PackageFullNameFromFamilyName(std::wstring familyName)
{
	std::wstring fullName;
	UINT32 count = 0;
	UINT32 length = 0;

	// First call gets the count and length; PACKAGE_FILTER_HEAD tells Windows to query Application Packages
	LONG status = FindPackagesByPackageFamily(familyName.c_str(), PACKAGE_FILTER_HEAD, &count, nullptr, &length, nullptr, nullptr);
	if (status == ERROR_SUCCESS || status != ERROR_INSUFFICIENT_BUFFER)
		return fullName;

	PWSTR * fullNames = (PWSTR *)malloc(count * sizeof(*fullNames));
	if (fullNames == nullptr)
		goto Cleanup;

	PWSTR buffer = (PWSTR)malloc(length * sizeof(WCHAR));
	if (buffer == nullptr)
		goto Cleanup;

	UINT32 * properties = (UINT32 *)malloc(count * sizeof(*properties));
	if (properties == nullptr)
		goto Cleanup;

	// Second call gets all fullNames
	// buffer and properties are needed even though they're never used
	status = FindPackagesByPackageFamily(familyName.c_str(), PACKAGE_FILTER_HEAD, &count, fullNames, &length, buffer, properties);
	if (status != ERROR_SUCCESS)
		goto Cleanup;
	else
		fullName = std::wstring(fullNames[0]); // Get the first activatable package found; usually there is only one anyway

Cleanup:
	if (properties != nullptr)
		free(properties);
	if (buffer != nullptr)
		free(buffer);
	if (fullNames != nullptr)
		free(fullNames);

	return fullName;
}

std::wstring AppIdFromFullName(std::wstring fullName)
{
	std::wstring appId;
	PACKAGE_INFO_REFERENCE pir = { 0 };

	// Get PackageInfo using the supplied PackageFullName
	LONG rc = OpenPackageInfoByFullName(fullName.c_str(), 0, &pir);
	if (rc != ERROR_SUCCESS)
		return appId;

	UINT32 count;
	UINT32 length = 0;

	// First call gets the length and count
	rc = GetPackageApplicationIds(pir, &length, NULL, &count);
	if (rc != ERROR_INSUFFICIENT_BUFFER)
	{
		ClosePackageInfo(pir);
		return appId;
	}

	// Allocate buffer space to store the ApplicationId's
	BYTE* buffer = (BYTE*)malloc(length);
	if (buffer == NULL)
	{
		ClosePackageInfo(pir);
		return appId;
	}

	// Second Call gets the ApplicationId's and stores them in the buffer
	rc = GetPackageApplicationIds(pir, &length, buffer, &count);
	if (rc != ERROR_SUCCESS)
	{
		ClosePackageInfo(pir);
		return appId;
	}

	PCWSTR* applicationUserModelIds = (PCWSTR*)buffer;

	// Set the appId as the first ID found; normally apps only have one AppId
	appId = applicationUserModelIds[0];

	free(buffer);

	// Close the package info opened earlier
	rc = ClosePackageInfo(pir);
	if (rc != ERROR_SUCCESS)
		return appId;

	return appId;
}
