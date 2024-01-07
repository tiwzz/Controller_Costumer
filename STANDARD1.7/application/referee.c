#include "referee.h"
#include "string.h"
#include "stdio.h"
#include "CRC8_CRC16.h"
#include "protocol.h"
#include "usart.h"
#include "cmsis_os.h"
#include "senddatatask.h"
#define MAX_SIZE 128      // �ϴ��������ĳ���
#define frameheader_len 5 // ֡ͷ����
#define cmd_len 2         // �����볤��
#define crc_len 2         // CRC16У��

frame_header_struct_t referee_receive_header;
frame_header_struct_t referee_send_header;

ext_game_state_t game_state;
ext_game_result_t game_result;
ext_game_robot_HP_t game_robot_HP_t;

ext_event_data_t field_event;
ext_supply_projectile_action_t supply_projectile_action_t;
ext_supply_projectile_booking_t supply_projectile_booking_t;
ext_referee_warning_t referee_warning_t;

ext_game_robot_state_t robot_state;
ext_power_heat_data_t power_heat_data_t;
ext_game_robot_pos_t game_robot_pos_t;
ext_buff_musk_t buff_musk_t;
aerial_robot_energy_t robot_energy_t;
ext_robot_hurt_t robot_hurt_t;
ext_shoot_data_t shoot_data_t;
ext_bullet_remaining_t bullet_remaining_t;
ext_student_interactive_data_t student_interactive_data_t;

ext_rfid_status_t rfid_status_t;

custom_robot7_data_t customcontroller_tx; // ͼ����·
custom_robot7_data_t customcontroller_rx;
extern UART_HandleTypeDef huart1;
Controller_t_Rx rx_data_t;
void init_referee_struct_data(void)
{
    memset(&referee_receive_header, 0, sizeof(frame_header_struct_t));
    memset(&referee_send_header, 0, sizeof(frame_header_struct_t));

    memset(&game_state, 0, sizeof(ext_game_state_t));
    memset(&game_result, 0, sizeof(ext_game_result_t));
    memset(&game_robot_HP_t, 0, sizeof(ext_game_robot_HP_t));

    memset(&field_event, 0, sizeof(ext_event_data_t));
    memset(&supply_projectile_action_t, 0, sizeof(ext_supply_projectile_action_t));
    memset(&supply_projectile_booking_t, 0, sizeof(ext_supply_projectile_booking_t));
    memset(&referee_warning_t, 0, sizeof(ext_referee_warning_t));

    memset(&robot_state, 0, sizeof(ext_game_robot_state_t));
    memset(&power_heat_data_t, 0, sizeof(ext_power_heat_data_t));
    memset(&game_robot_pos_t, 0, sizeof(ext_game_robot_pos_t));
    memset(&buff_musk_t, 0, sizeof(ext_buff_musk_t));
    memset(&robot_energy_t, 0, sizeof(aerial_robot_energy_t));
    memset(&robot_hurt_t, 0, sizeof(ext_robot_hurt_t));
    memset(&shoot_data_t, 0, sizeof(ext_shoot_data_t));

    memset(&bullet_remaining_t, 0, sizeof(ext_bullet_remaining_t));

    memset(&student_interactive_data_t, 0, sizeof(ext_student_interactive_data_t));

    memset(&rx_data_t, 0, sizeof(Controller_t_Rx)); // ͼ����·
}

void referee_data_solve(uint8_t *frame)
{
    uint16_t cmd_id = 0;

    uint8_t index = 0;

    memcpy(&referee_receive_header, frame, sizeof(frame_header_struct_t));

    index += sizeof(frame_header_struct_t);

    memcpy(&cmd_id, frame + index, sizeof(uint16_t));
    index += sizeof(uint16_t);
    switch (cmd_id)
    {
    case GAME_STATE_CMD_ID:
    {
        memcpy(&game_state, frame + index, sizeof(ext_game_state_t));
    }
    break;
    case GAME_RESULT_CMD_ID:
    {
        memcpy(&game_result, frame + index, sizeof(game_result));
    }
    break;
    case GAME_ROBOT_HP_CMD_ID:
    {
        memcpy(&game_robot_HP_t, frame + index, sizeof(ext_game_robot_HP_t));
    }
    break;
    case RFID_STATE_CMD_ID:
    {
        memcpy(&rfid_status_t, frame + index, sizeof(rfid_status_t));
    }
    case FIELD_EVENTS_CMD_ID:
    {
        memcpy(&field_event, frame + index, sizeof(field_event));
    }
    break;
    case SUPPLY_PROJECTILE_ACTION_CMD_ID:
    {
        memcpy(&supply_projectile_action_t, frame + index, sizeof(supply_projectile_action_t));
    }
    break;
    case SUPPLY_PROJECTILE_BOOKING_CMD_ID:
    {
        memcpy(&supply_projectile_booking_t, frame + index, sizeof(supply_projectile_booking_t));
    }
    break;
    case REFEREE_WARNING_CMD_ID:
    {
        memcpy(&referee_warning_t, frame + index, sizeof(ext_referee_warning_t));
    }
    break;

    case ROBOT_STATE_CMD_ID:
    {
        memcpy(&robot_state, frame + index, sizeof(robot_state));
    }
    break;
    case POWER_HEAT_DATA_CMD_ID:
    {
        memcpy(&power_heat_data_t, frame + index, sizeof(power_heat_data_t));
    }
    break;
    case ROBOT_POS_CMD_ID:
    {
        memcpy(&game_robot_pos_t, frame + index, sizeof(game_robot_pos_t));
    }
    break;
    case BUFF_MUSK_CMD_ID:
    {
        memcpy(&buff_musk_t, frame + index, sizeof(buff_musk_t));
    }
    break;
    case AERIAL_ROBOT_ENERGY_CMD_ID:
    {
        memcpy(&robot_energy_t, frame + index, sizeof(robot_energy_t));
    }
    break;
    case ROBOT_HURT_CMD_ID:
    {
        memcpy(&robot_hurt_t, frame + index, sizeof(robot_hurt_t));
    }
    break;
    case SHOOT_DATA_CMD_ID:
    {
        memcpy(&shoot_data_t, frame + index, sizeof(shoot_data_t));
    }
    break;
    case BULLET_REMAINING_CMD_ID:
    {
        memcpy(&bullet_remaining_t, frame + index, sizeof(ext_bullet_remaining_t));
    }
    break;
    case STUDENT_INTERACTIVE_DATA_CMD_ID:
    {
        memcpy(&student_interactive_data_t, frame + index, sizeof(student_interactive_data_t));
    }
    break;
    case IDCustomDatarx:
    {
        memcpy(&rx_data_t, frame + index, sizeof(Controller_t_Rx)); // ͼ����·
    }
	break;
    default:
    {
        break;
    }
    }
}

void get_chassis_power_and_buffer(fp32 *power, fp32 *buffer)
{
    *power = power_heat_data_t.chassis_power;
    *buffer = power_heat_data_t.chassis_power_buffer;
}

uint8_t get_robot_id(void)
{
    return robot_state.robot_id;
}

void get_shoot_heat_limit_and_heat(uint16_t *heat_limit, uint16_t *heat)
{
    *heat_limit = robot_state.shooter_id1_42mm_cooling_limit;
    *heat = power_heat_data_t.shooter_id1_42mm_cooling_heat;
}

void customcontroller_data_pack_send(uint8_t *p_data, uint16_t len)
{
		memset(&customcontroller_tx.given_current, 0, sizeof(customcontroller_tx.given_current));
		static uint8_t seq = 0;
    uint16_t frame_length = 39; // ����֡����

    customcontroller_tx.frame_header.SOF = 0xA5;                                          // ����֡��ʼ�ֽ�
    customcontroller_tx.frame_header.data_length = len;                                   // ������data�ĳ���
    customcontroller_tx.frame_header.seq = seq++;                                           // �����
    append_CRC8_check_sum((uint8_t *)(&customcontroller_tx.frame_header), frameheader_len); // ֡ͷУ��CRC8

    customcontroller_tx.IDCustomData = 0x0302; // ������

		memcpy(&customcontroller_tx.given_current, p_data, 30);
    //customcontroller_tx.given_current = *p_data; // �Զ���������������������
    append_CRC16_check_sum((uint8_t *)&customcontroller_tx, frame_length);

    // if (seq == 0xff)
    //     seq = 0;
    // else
    //     seq++;

//    uint32_t wait_time = xTaskGetTickCount();
    HAL_UART_Transmit(&huart1, (uint8_t *)(&customcontroller_tx), sizeof(custom_robot7_data_t), 50);
//    osDelayUntil(&wait_time, 500);
}
