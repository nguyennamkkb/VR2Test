#include "vimass.h"
void get_tx_cks(int16 len)
{
    uint8 *ptr;
    uint16 cks = 0;
    if (len <= 2)
    {
        ptr = cmd_tx.prefix;
        for (int i = 0; i < (int)(sizeof(cmd_tx)) - 2; i++)
        {
            cks += *ptr++;
        }
        cmd_tx.cks = cks;
    }
    else if (len == 498)
    {
        ptr = data_tx.prefix;
        for (int i = 0; i < 6 + len; i++)
        {
            cks += *ptr++;
        }
        data_tx.data[len >> 1] = cks;
    }
}

int16 get_rx_cks(int16 len)
{
    int16 cks = 0;
    uint8 *ptr;
    if (len <= 2)
    {
        ptr = cmd_rx.prefix;
        for (int i = 0; i < (int)(sizeof(cmd_rx)) - 2; i++)
        {
            cks += *ptr++;
        }
        return (cmd_rx.cks == cks);
    }
    else
    {
        ptr = data_rx.prefix;
        for (int i = 0; i < 6 + len; i++)
        {
            cks += *ptr++;
        }
        return (cks == data_tx.data[len >> 1]);
    }
}

int16 get_cmd_len(int16 cmd)
{
    for (int i = 0; i < 38; i++)
    {
        if (cmd_len[i * 2] == cmd)
        {
            return cmd_len[i * 2 + 1];
        }
    }
    return 0;
}
void cmd_send(int16 cmd, int16 len, int16 page_id)
{
    switch (cmd)
    {
    case Verify:
    case Identify:
    case Enroll_3:
    case Enroll_1:
    case Get_Feature:
    case Verify_Feature:
    case Enroll_RAM:
    // case Set_Security:
    // case Get_Security:
    case Identify_Feature:
        fp_auto_off = 250;
        break;
    default:
        break;
    }
    switch (len)
    {
    case 0:
    case 2:
        cmd_tx.prefix[0] = cmd_prefix & 0xff;
        cmd_tx.prefix[1] = (cmd_prefix >> 8) & 0xff;
        cmd_tx.cmd = cmd;
        cmd_tx.len = len;
        memset(cmd_tx.data, 0, sizeof(cmd_tx.data));
        if (cmd == Verify_Feature)
        {
            cmd_tx.data[0] = 498;
            memcpy(char_buf, udp_rx.data, char_buf_len);
        }
        else if (cmd == Set_Security)
        {
            cmd_tx.data[0] = 2;
            cmd_tx.data[1] = 0;
            // memset(cmd_tx.data, 0, sizeof(cmd_tx.data));
        }
        else if (cmd == Get_Security)
        {
            cmd_tx.data[0] = 0;
            cmd_tx.data[1] = 0;
            // memset(cmd_tx.data, 0, sizeof(cmd_tx.data));
        }
        else if (len)
        {
            cmd_tx.data[0] = page_id;
        }
        get_tx_cks(len);
    case 14:
        break;
    case 498:
        data_tx.prefix[0] = data_prefix & 0xff;
        data_tx.prefix[1] = (data_prefix >> 8) & 0xff;
        data_tx.cmd = cmd;
        data_tx.len = len;
        memcpy(data_tx.data, char_buf, char_buf_len);
        get_tx_cks(len);
        break;
    }
      if (cmd ==  Enroll_1 || cmd == Enroll_3)
      {
        delay(1000);
        Play_voice(Moi_dat_van_tay);
        delay(1000);
      }
}
bool cmd_found(int16 cmd)
{
    bool b = false;
    for (int i = 0; i < sizeof(cmd_len) / 4; i++)
    {
        if (cmd == cmd_len[i * 2])
        {
            b = true;
            i = sizeof(cmd_len);
        }
    }
    return b;
}
void cmd_switch()
{
    rx_cnt = 0;
    // String sss = "";
    // for (int i = 0; i < sizeof(udp_in_buf); i++)
    // {
    //     sss += udp_in_buf[i];

    // }
    // uart0.println("udp_rx.cmd:"+String(udp_rx.cmd));
    String s = "";
    int16 len = 0;
    // if(cmd_found(udp_rx.cmd)){

    if ((udp_rx.cmd > 0x100) && (udp_rx.cmd < 0x151))
    {
        cmd_send(udp_rx.cmd, udp_rx.len, udp_rx.page_id);
        uart1.write(cmd_tx.prefix, sizeof(cmd_tx));
        led_reset = 5;
        return;
    }
    switch (udp_rx.cmd)
    {
    case voice_download:
    // uart0.println("dang tai xuong");
        if (udp_rx.seg_type == seg_save)
        {
            len = udp_rx.seg_len;
            if (len > ima_seg_len)
                len = ima_seg_len;
            memcpy(&ima_buf[voice_len], udp_rx.data, len);
            voice_len += len;

            remove("/a_" + String(udp_rx.page_id & 0xf) + ".wav");//

            if (write_buf("/a_" + String(udp_rx.page_id & 0xf) + ".wav", ima_buf, voice_len))
                led_reset = 10;
        }
        else if (voice_len < (ima_seg_max - 1) * ima_seg_len)
        {
            if (udp_rx.seg_type == seg_begin)
            {
                voice_len = 0;
            }
            len = udp_rx.seg_len;
            if (len > ima_seg_len)
                len = ima_seg_len;
            memcpy(&ima_buf[voice_len], udp_rx.data, len);
            voice_len += len;
        }
        memcpy(udp_tx.cmd_rx.prefix, cmd_rx.prefix, sizeof(cmd_rx));
        udp_tx.pkt_type = FS01_CMD;
        udp_tx.cmd_rx.rcm = udp_rx.cmd;
        udp_tx.cmd_rx.len = voice_len;
        uni_send(&udp_tx.bcc, sizeof(udp_tx));
        break;
    case voice_play:
        if (read_buf("/a_" + String(udp_rx.page_id & 0xf) + ".wav"))
        {
            voice_init();
        }

        // remove("/a_" + String(udp_rx.page_id & 0xf) + ".wav");//xoa file

        break;
    case file_rename:
        break;
    case file_list:
        string_send(list_file());
        break;
    case set_bip_on:
        xacthucvantay = 1; 
        reset_7462("2xx3"); // du dang lam
        
        break;
    case send_ev2:
        // reset_7462("2x7");
        // delay(1000);
        memcpy(char_buf, udp_rx.data, char_buf_len);
        char_buf_to_ev2();

        break;
    case add_ssid:
        String fl ;
 
        for (int i = 0; i < 10; i++)
        {
            fl = "/a_"+String(i)+".wav";
            if (remove(fl))
            {
                uart0.println("Xoa"+fl+" thanh cong");
            }else{
                uart0.println("Xoa "+fl+"  khong thanh cong");
            }
        }
        
        break;
    }
}
void voice_init()
{
    d_predsample = 0;
    d_index = 0;
    ima_buf_cnt = 0;
    ima_haft = 0;
    timer_init(true);
}
