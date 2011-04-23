/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2011 by Christoph Schueler <github.com/Christoph2,
 *                                       cpu12.gems@googlemail.com>
 *
 *   All Rights Reserved
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
#if !defined(__KNX_LAYER_APPLICATION_H)
#define __KNX_LAYER_APPLICATION_H

#include "knx_messaging.h"
#include "KNXConv.h"
#include "knx_address.h"
#include "knx_layer_transport.h"
#include "knx_ios.h"
#include "knx_lsm.h"
#include "knx_apci.h"
#include "Appl.h"

extern const uint8 KNX_OBJ_LEN_TAB[];

extern const uint8 APP_CommObjTab[];

extern uint8    DEV_Device_Control;
extern uint8    DEV_Current_Accesslevel;
extern uint8    DEV_SystemState;

#define AL_GetCommObjDescr(objectNumber)        ((KNX_CommObjDescriptorType *)((uint8 *)APP_CommObjTab + (uint8)2) + (objectNumber))
#define AL_GetNumCommObjs()                     ((uint8)APP_CommObjTab[0])

uint8 * AL_GetObjectDataPointer(uint8 objectNr);

#define AL_ObjCheckEnabled(f, m)                (((f) & (m)) == (m))

#define AL_ObjTransmitEnabled(f)                AL_ObjCheckEnabled((f), (KNX_OBJ_COMM_ENABLE | KNX_OBJ_TRANSMIT_ENABLE))
#define AL_ObjReadEnabled(f)                    AL_ObjCheckEnabled((f), (KNX_OBJ_COMM_ENABLE | KNX_OBJ_READ_ENABLE))
#define AL_ObjResponseEnabled(f)                AL_ObjCheckEnabled((f), \
                                                                   (KNX_OBJ_COMM_ENABLE | KNX_OBJ_WRITE_ENABLE | \
                                                                    KNX_OBJ_UPDATE_ENABLE))
#define AL_ObjWriteEnabled(f)                   AL_ObjCheckEnabled((f), (KNX_OBJ_COMM_ENABLE | KNX_OBJ_WRITE_ENABLE))

#define AL_GetTransmissionStatus(objectNr)      (AL_GetRAMFlags((objectNr)) & (uint8)0x03)
#define AL_IsObjectTransmitting(objectNr)       ((AL_GetTransmissionStatus((objectNr)) & KNX_OBJ_TRANSMIT_REQ) == \
                                                 KNX_OBJ_TRANSMIT_REQ)
#define AL_IsObjectBusy(objectNr)               (AL_IsObjectTransmitting((objectNr)))

#define AL_GetObjLen(o)                         (KNX_OBJ_LEN_TAB[((o) & (uint8)0x3f)])
#define AL_GetObjPriority(objectNr)             ((KNX_PriorityType)(AL_GetCommObjDescr((objectNr))->Config) & (uint8)0x03)

#define AL_GetAPDUShortData(pmsg, nbits)        ((uint8)(pmsg)->apci & (uint8)KNX_AL_SHORT_DATA_MASK[(nbits) - (uint8)1])
#define AL_SetAPDUShortData(pmsg, data, nbits)  ((pmsg)->apci = \
                                                     ((pmsg)->apci & \
                                                      ~(uint8)0x3f) | ((data) & KNX_AL_SHORT_DATA_MASK[(nbits) - (uint8)1]))

void    AL_SetRAMFlags(uint16 objectNr, uint8 flags);
uint8   AL_GetRAMFlags(uint16 objectNr);
uint8 * AL_GetRAMFlagPointer(void);

void AL_UpdateAssociatedASAPs(PMSG_Buffer pBuffer, uint8 testFlags);

void    DEV_SetVerifyMode(boolean on);
void    DEV_ChkOwnPhysAddrRcvd(boolean on);
void    DEV_PhysAddrWriteEnable(boolean on);

void    ALG_Task(void);
void    ALM_Task(void);
void    ALG_PollCycle(void);

/*  void AL_Init(void); */
void ALG_Init(void);

uint8 AL_GetAPCIType(const KNX_StandardFrameRefType pmsg);

void    AL_GetAPDUData(const KNX_StandardFrameRefType pmsg, uint8 offset, uint8 * data, uint8 len);
void    AL_SetAPDUData(const KNX_StandardFrameRefType pmsg, uint8 offset, uint8 * data, uint8 len);
uint8   AL_GetAPDUDataByte(const KNX_StandardFrameRefType pmsg, uint8 offset);
void    AL_SetAPDUDataByte(const KNX_StandardFrameRefType pmsg, uint8 offset, const uint8 value);

/*
**  Application-Layer-Services.
*/

/* Group-Services. */

void    A_GroupValue_Read_Req(PMSG_Buffer pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8 prio);
void    A_GroupValue_Write_Req(PMSG_Buffer pBuffer, Knx_AddressType source, Knx_AddressType dest, KNX_PriorityType prio,
                               uint8 * data,
                               uint8 len);

/* Broadcast-Services. */

void    A_IndividualAddress_Read_Res(PMSG_Buffer pBuffer, Knx_AddressType source);
void    A_IndividualAddress_Read_Req(PMSG_Buffer pBuffer, Knx_AddressType source);
void    A_IndividualAddress_Write_Req(PMSG_Buffer pBuffer, Knx_AddressType source, Knx_AddressType newaddress);

void A_IndividualAddressSerialNumber_Read_Res(PMSG_Buffer pBuffer, Knx_AddressType source, Knx_SerialNumberType serial_number,
                                              Knx_AddressType domain_addr);
void    A_IndividualAddressSerialNumber_Read_Req(PMSG_Buffer pBuffer, Knx_AddressType source, Knx_SerialNumberType serial_number);
void    A_IndividualAddressSerialNumber_Write_Req(PMSG_Buffer pBuffer, Knx_AddressType source, Knx_SerialNumberType serial_number,
                                                  Knx_AddressType new_addr);

void A_ServiceInformation_Indication_Write_Req(PMSG_Buffer pBuffer, Knx_AddressType source, uint8 info);

void    A_DomainAddress_Read_Res(PMSG_Buffer pBuffer, Knx_AddressType source);
void    A_DomainAddress_Read_Req(PMSG_Buffer pBuffer, Knx_AddressType source);
void    A_DomainAddress_Write_req(PMSG_Buffer pBuffer, Knx_AddressType source, Knx_AddressType domain_ddress);
void    A_DomainAddressSelective_Read_req(PMSG_Buffer pBuffer, Knx_AddressType source, Knx_AddressType domain_ddress,
                                          Knx_AddressType start_address, uint8 range);

void A_PropertyValue_Read_Res(PMSG_Buffer pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8 obj_index,
                              uint8 prop_id, uint8 nr_of_elem, uint16 start_index, uint8 * data);
void A_PropertyValue_Read_Res_NoData(PMSG_Buffer pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8 obj_index,
                                     uint8 prop_id, uint8 nr_of_elem, uint16 start_index);
void A_PropertyValue_Read_Req(PMSG_Buffer pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8 obj_index,
                              uint8 prop_id, uint8 nr_of_elem, uint16 start_index);

/* todo: Prop_Write!!! */

void A_PropertyDescription_Read_Res(PMSG_Buffer pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8 obj_index,
                                    uint8 prop_id, uint8 prop_index, uint8 type, uint16 nr_of_elem, uint8 access);
void A_PropertyDescription_Read_Res_NoData(PMSG_Buffer pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8 obj_index,
                                           uint8 prop_id, uint8 prop_index);
void A_PropertyDescription_Read_Req(PMSG_Buffer pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8 obj_index,
                                    uint8 prop_id, uint8 prop_index);

#endif  /* __KNX_LAYER_APPLICATION_H */
