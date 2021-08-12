/*******************************************************************************
*
*    FILE: uvc_sdk.c
*
*    DESCRIPTION: uvc SDK for lepton uvc mode
*
*    AUTHOR: Kyungmin Lee
*
*    CREATED: 25/5/2021
*
*    HISTORY: 25/5/2021 Initial draft
*
**  Functions, definitions, types copied from leptonvariation.
**
*******************************************************************************/

#include <stdio.h>
#include "uvc_sdk.h"
#include "LEPTON_SDK.h"
#include "LEPTON_AGC.h"

#ifdef LEP_USE_DYNAMIC_ALLOCATION
    #include "stdlib.h"
#endif

#define LEP_CID_AGC_MODULE (0x0100)
#define LEP_CID_OEM_MODULE (0x0800)
#define LEP_CID_RAD_MODULE (0x0E00)
#define LEP_CID_SYS_MODULE (0x0200)
#define LEP_CID_VID_MODULE (0x0300)

typedef enum {
  VC_CONTROL_XU_LEP_AGC_ID = 3,
  VC_CONTROL_XU_LEP_OEM_ID,
  VC_CONTROL_XU_LEP_RAD_ID,
  VC_CONTROL_XU_LEP_SYS_ID,
  VC_CONTROL_XU_LEP_VID_ID,
} VC_TERMINAL_ID;

LEP_RESULT UVC_GetAttribute(LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
                            LEP_COMMAND_ID commandID, 
                            LEP_ATTRIBUTE_T_PTR attributePtr,
                            LEP_UINT16 attributeWordLength)
{
    int unit_id;
    int control_id;
    int result;

    uvc_device_handle_t *devh = (uvc_device_handle_t*)portDescPtr->userPtr;

    unit_id = leptonCommandIdToUnitId(commandID);

    if (unit_id < 0)
        return (LEP_RESULT)unit_id;

    control_id = ((commandID & 0x00ff) >> 2) + 1;
    attributeWordLength *= 2;

    //printf ("unit_id: %d\t, control_id: %d\n", unit_id, control_id);
    result = uvc_get_ctrl(devh, unit_id, control_id, attributePtr, attributeWordLength, UVC_GET_CUR);

    if (result != attributeWordLength) {
        printf("UVC_GetAttribute failed: %d", result);
        return LEP_COMM_ERROR_READING_COMM;
    }

    return LEP_OK;
}

LEP_RESULT UVC_SetAttribute(LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
                            LEP_COMMAND_ID commandID,
                            LEP_ATTRIBUTE_T_PTR attributePtr,
                            LEP_UINT16 attributeWordLength)
{
    int unit_id;
    int control_id;
    int result;

    uvc_device_handle_t *devh = (uvc_device_handle_t*)portDescPtr->userPtr;

    unit_id = leptonCommandIdToUnitId(commandID);

    if (unit_id < 0)
        return (LEP_RESULT)unit_id;

    control_id = ((commandID & 0x00ff) >> 2) + 1;
    attributeWordLength *= 2;

    //printf ("unit_id: %d\t, control_id: %d\n", unit_id, control_id);
    result = uvc_set_ctrl(devh, unit_id, control_id, attributePtr, attributeWordLength);

    if (result != attributeWordLength) {
        printf("UVC_SetAttribute failed: %d", result);
        return LEP_COMM_ERROR_READING_COMM;
    }

    return LEP_OK;
}

LEP_RESULT UVC_RunCommand(LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
                          LEP_COMMAND_ID commandID)
{
    int unit_id;
    int control_id;
    int result;

    uvc_device_handle_t *devh = (uvc_device_handle_t*)portDescPtr->userPtr;

    unit_id = leptonCommandIdToUnitId(commandID);

    if (unit_id < 0)
        return (LEP_RESULT)unit_id;

    control_id = ((commandID & 0x00ff) >> 2) + 1;

    result = uvc_set_ctrl(devh, unit_id, control_id, &control_id, 1);

    if (result != 1) {
        printf("UVC_RunCommand failed: %d", result);
        return LEP_COMM_ERROR_READING_COMM;
    }

    return LEP_OK;
}

int leptonCommandIdToUnitId(LEP_COMMAND_ID commandID)
{
    int unit_id;

    switch (commandID & 0x3f00) // Ignore upper 2 bits including OEM bit
    {
    case LEP_CID_AGC_MODULE:
        unit_id = VC_CONTROL_XU_LEP_AGC_ID;
        break;

    case LEP_CID_OEM_MODULE:
        unit_id = VC_CONTROL_XU_LEP_OEM_ID;
        break;

    case LEP_CID_RAD_MODULE:
        unit_id = VC_CONTROL_XU_LEP_RAD_ID;
        break;

    case LEP_CID_SYS_MODULE:
        unit_id = VC_CONTROL_XU_LEP_SYS_ID;
        break;

    case LEP_CID_VID_MODULE:
        unit_id = VC_CONTROL_XU_LEP_VID_ID;
        break;

    default:
        return LEP_RANGE_ERROR;
    }

    return unit_id;
}

