#ifndef __SEND_DATA_TASK__
#define __SEND_DATA_TASK__

#define FRAME_HEADER_LENGTH 5 // ֡ͷ���ݳ���
#define CMD_ID_LENGTH 2       // ������ID���ݳ���
#define DATA_LENGTH 30        // ���ݶγ���
#define FRAME_TAIL_LENGTH 2   // ֡β���ݳ���

#define DATA_FRAME_LENGTH (FRAME_HEADER_LENGTH + CMD_ID_LENGTH + DATA_LENGTH + FRAME_TAIL_LENGTH) // ��������֡�ĳ���

#define CONTROLLER_CMD_ID 0x0302 // �Զ��������������   
#endif
