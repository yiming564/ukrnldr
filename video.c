#include "shared.h"

EFI_STATUS EFIAPI VideoInit(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gGraphicsOutput = 0;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* Info = 0;
	UINTN InfoSize = 0;
	int i = 0;

	gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **)&gGraphicsOutput);
	
	long H_V_Resolution = gGraphicsOutput->Mode->Info->HorizontalResolution * gGraphicsOutput->Mode->Info->VerticalResolution;
	int MaxResolutionMode = gGraphicsOutput->Mode->Mode;

	for(i = 0; i < gGraphicsOutput->Mode->MaxMode; i++)
	{
		gGraphicsOutput->QueryMode(gGraphicsOutput,i,&InfoSize,&Info);

		if((Info->PixelFormat == 1) && (Info->HorizontalResolution * Info->VerticalResolution > H_V_Resolution))
		{
			H_V_Resolution = Info->HorizontalResolution * Info->VerticalResolution;
			MaxResolutionMode = i;
		}

		gBS->FreePool(Info);
	}

	gGraphicsOutput->SetMode(gGraphicsOutput, MaxResolutionMode);
	gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **)&gGraphicsOutput);
	Print(L"Video Mode Has Been Set\n");
	Print(L"Current Mode: %02d, Version: %x, Format: %d, Horizontal: %d, Vertical: %d, ScanLine: %d, FrameBufferBase: %010lx, FrameBufferSize: %010lx\n", \
	gGraphicsOutput->Mode->Mode, gGraphicsOutput->Mode->Info->Version, gGraphicsOutput->Mode->Info->PixelFormat, \
	gGraphicsOutput->Mode->Info->HorizontalResolution, gGraphicsOutput->Mode->Info->VerticalResolution, \
	gGraphicsOutput->Mode->Info->PixelsPerScanLine, gGraphicsOutput->Mode->FrameBufferBase, gGraphicsOutput->Mode->FrameBufferSize);

	gBS->CloseProtocol(gGraphicsOutput, &gEfiGraphicsOutputProtocolGuid, ImageHandle, NULL);
	Print(L"Video Module Finished\n\n");
	return EFI_SUCCESS;
}