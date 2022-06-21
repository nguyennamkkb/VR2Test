// strcpy(ssid_name,str_name.c_str());
// String(int,HEX); dec to HEX
// n=strtol(s.c_str(),NULL,16); HEX to decimal
#include "vimass.h"

#include "SPIFFS.h" // #include <dummy.h>
#include <Math.h>
#include <dummy.h>
#include <WiFi.h>
#include "esp_attr.h"
#include "HardwareSerial.h"
#include <ArduinoOTA.h>
#include <WiFiMulti.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESPmDNS.h>
#include <MD5.h>

WiFiMulti wifiMulti;
hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
// WiFiUDP myUDP;
// WiFiUDP ntpUDP;

IPAddress apIP(192, 168, 2, 2), ap_gateway(192, 168, 2, 1), bcast_ip(192, 168, 2, 255), my_ip, pc_ip, esp2_ip,myIP,pc_IP,bcast_IP,tmp_IP;;
WiFiUDP myUDP;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

int16 led_cnt, led1_on, led2_on, led_reset = 5, ms200 = 0, ms1000 = 0, pc_ip_cnt = 0;
int16 ms10, pc_on, wifi_status, string_valid_cnt, ms2;
int16 timer_1ms, udp_time = 0;
int16 wifi_on, file_ok, udp_on = 0, serial_on = 0;
int16 new_millis, old_millis, rx2_time_out;
int8 demghi = 0;
int64_t ms64_0, ms64, delta64;
int16 v_stat = 0, msg_type;
uint8 udp_in_buf[1400];
int16 my_id, hd_len;
int16 ms5, fp_off = 0, fp_on = 0, fp_detect_cnt = 0, fp_off_delay = 300, fp_mode_delay = 0, fp_fail = 0, fp_off_max = 500;
int16 tx_pkt_len, hd_index, buf_id, match_delay = 0, pulse_width = 300, pulse_max = 3;
uint8 char_buf[char_buf_len];
int16 data_pkt_cnt, door_open_delay = 0;
int16 p_millis, n_millis, d_millis;
uint16 r_port = 6000, solannhapmk = 0;
int16 rs485_tx_out = 0;
// ========vua nhap pass gui confirm
int16 vuaNhapPassConfirm = 0;
String s_log = "", magiaodich = "", str_door = "", str_lenhtbngoai = "", strdvtt = "", strdichvumaytram = "";
String idDelaySend[1]; // mã giao dịch kèm theo mật khẩu
String my_mac, serial_rs485, dulieughixacthuc = "", dau_dlxacthuc = "";
String iplocal[1];
String fp_save[5];
String strip="";
int trangthailenhtumaytinh = 0;
int ghivantay = 0;
int play_index = -1;
// int demghivt = 0;
String uid_the = "", vid_the = "", ma_tbi = "", data_chamcong = ""; // uid cua the
// == ip may chu mang noi bo
String tbttid[1];
// ======tham so confirm
String idDelayConfirm[1];
// ========dich vu F1
String dataServices[1];
uint8 datavantay[500];
uint8 rx_buf[rx_max];
// uint8 datavantay[rx_max];
String serial_in, params_str, serial2_out, serial2_in, vantay = "", khuonmat = "";
int16 page_id;
int16 hd_ok = 0, rx_pkt_len, rx_cnt = 0, pkt_type, check_sum, check_sum_ok;
uint8 ima_buf[ima_buf_max];
int16 voice_len, pwm_val, pwm_on = 0, ima_buf_cnt = 0, sin_cnt = 0, ima_haft = 0, pwm_send = 0;
uint8 pwm_tmp;
int16 n_prefix, response, ret, ret_code, door_cnt, fp_index = 0;
int16 bip_on = 0, fs01_on = 0, fp_auto_off, fp_on_duration, fp_on_max;
// bool TrangThaiThanhToanDich
bool Validate = false;
bool TrangThaiThanhToan = false;
bool DichVuMayTram = false;
bool TrangThaiCacheCard = false;
bool xacthucvantay = 0, xacthuckhuonmat = 0, xacthutiengnoi = 0, ghi_ma_tbi = 1;
bool auto_docvt = 1;
// int demghivt =1;
char ssid_name[32], ssid_pass[32];
String ds_name[ds_max][sub_ds_max];
String str_name[4], str_pass[4];
String passDelaySend[1];
int soluongthietbi = 50000;
int32 e_predsample, d_predsample;
int16 e_index, d_index;
int duong_ra = 0, hang_doi_front = 0, hang_doi_rear = 0, hang_doi_sothietbidangdoi = 0, hang_doi_sothietbidangdoi2 = 0; // phan tu dau, phan tu cuoi
#define hang_doi_tong 3
String hang_doi[hang_doi_tong];
int16 encode_buf[2048];
cmd_tx_struct cmd_tx;
cmd_rx_struct cmd_rx;
data_tx_struct data_tx;
data_rx_struct data_rx;

udp_rx_header udp_rx;
udp_tx_header udp_tx;
int8 n_time[6];

int16 vtlen = 0, kmlen = 0;

struct struct_max
{
	int16 fx;
	int16 lcd_line;
	int16 dv_line0;
	int16 dv_index;
	int16 dv_index_max;
	int16 dv_td_index;
	int16 dv_td_index_max;
	int16 line_type[8];
	int16 ds_index;
	int16 ds_index_max;
	int16 sub_ds_index;
	int16 sub_ds_index_max[ds_max];
};
struct_max v_fx;

String dv_td[dv_max];

struct cfg_struct
{
	int16 media;
	int16 key_bounce;
	int16 key_inhibit;
	int16 brate0;
	int16 https_timeout;
	int16 page_delay[4];
};
cfg_struct v_cfg;
void setup()
{
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(COI, OUTPUT);
	pinMode(rs485_ctrl, OUTPUT);
	pinMode(door_gate, OUTPUT);
	pinMode(fp_detect_pin, INPUT_PULLDOWN);
	coi_off;
	off_led2;

	Serial.begin(115200, SERIAL_8N1);
	Serial2.begin(115200, SERIAL_8N1);
	Serial1.begin(115200, SERIAL_8N1, pin_rx1, pin_tx1);
	file_ok = (SPIFFS.begin(true)) ? 1 : 0;
	my_mac = WiFi.macAddress();
	ms64 = millis();
	memset(n_time, 0, sizeof(n_time));
	delay(1000);
	digitalWrite(12, 0);
	delay(1000);
	rs485_out;
	wifi_setup();
	ota_init();
	s_log = SPIFFS.begin(true) ? "FS Ok" : "FS fail";
	s_log += " size= " + String(SPIFFS.totalBytes() >> 10);
	s_log += " used= " + String(SPIFFS.usedBytes() >> 10);
	pwm_init();
	cmd_send(Set_Security, 2, 0);
	uart1.write(cmd_tx.prefix, sizeof(cmd_tx));
	send_https("56_" + tbttid[0]);
	read_iplocal();
	timeClient.begin();
	timeClient.setTimeOffset(+7 * 60 * 60);
	bipok1();
	// Nam
	//  unsigned char *hash = MD5::make_hash("hello world");
	//  // generate the digest (hex encoding) of our hash
	//  char *md5str = MD5::make_digest(hash, 16);
	//  // print it on our serial monitor
	//  uart0.println(md5str);
	readFile("/hello.txt");
	myUDP.begin(5000);
	my_id = 10000;
	save_id();
	
}
void IRAM_ATTR fp_isr()
{
}
void fp_test()
{
	if (auto_docvt)
	{
		if (fp_detect_cnt < 200)
			fp_detect_cnt++;
		fp_on_duration++;
	}
	else
	{
		if (fp_detect_cnt)
		{
			fp_detect_cnt--;
		}
		else
		{
			if (fp_on_duration)
			{
				fp_on_max = fp_on_duration;
				fp_on_duration = 0;
			}
		}
	}
	if (fp_detect_cnt >= 200)
	{
		fp_detect_cnt = 0;
		if (fp_auto_off == 0)
		{
			cmd_send(Identify, 0, 0);
			uart1.write(cmd_tx.prefix, sizeof(cmd_tx));
		}
	}
}
void loop()
{
	if (pwm_on == 0)
		ArduinoOTA.handle();
	millis_1();
}
void millis_1()
{
	new_millis = millis() & 0x3fff;
	if (old_millis == new_millis)
		return;
	old_millis = new_millis;
	String s;
	serial_test();
	serial2_test();
	serial2_out_test();
	digitalWrite(door_gate, 1); // cua dong
	UDP_receive();
	ms10 &= 0xf;
	if (++ms10 > 9) // vong lap 10ms
	{
		ms10 = 0;
		fp_test();
		int wifi_status = WiFi.status();
		if (wifi_status == WL_CONNECTED)
			wifi_on = 300;
		if (wifi_on)
			wifi_on--;
		else
			ESP.restart();
		// if(pwm_on)					led_reset=10;
		if (pc_ip_cnt)
		{
			pc_ip_cnt--;
		}
		else
		{
			pc_ip = bcast_ip;
			r_port = 6000;
		}
		if (fp_auto_off)
			fp_auto_off--;
		// if(door_open_delay)	door_open_delay--;
		// else 				door_close;
	}
	ms200 &= 0x1ff;
	if (++ms200 > 200) // vong 200ms kiem tra led
	{
		ms200 = 0;
		led_test();
	}
	ms1000 &= 0xfff;
	if (++ms1000 > 1000)
	{
		ms1000 = 0;
		// bcast_out();
	}
}
void bcast_out()
{
	// String s = s_log + " " + String(fp_on_max) + " " + String(fp_detect_cnt) + " " + String(fp_auto_off);
	// uint8 buf[s.length()];
	// udp_tx.pkt_type = FS01_BCAST;
	// udp_tx.data_rx.rcm = WiFi.RSSI();
	// udp_tx.data_rx.len = s.length();
	// udp_tx.cmd_rx.data[0] = my_ip[3];
	// udp_tx.cmd_rx.data[0] <<= 8;
	// udp_tx.cmd_rx.data[0] |= my_ip[3];
	// udp_tx.cmd_rx.data[1] = digitalRead(master_in);
	// for (int i = 0; i < udp_tx.data_rx.len; i++)
	// {
	// 	buf[i] = s.charAt(i);
	// }
	// memcpy(udp_tx.data_rx.data, buf, sizeof(buf));
	// bcast_send(&udp_tx.bcc, sizeof(udp_tx));
}
void led_test()
{
	led_cnt &= 0x1f;
	if (led_cnt <= 0)
	{
		led1_on = 0;
		if (wifi_on)
			led1_on++;
		if (pc_ip_cnt)
			led1_on = 2;
		if (fs01_on)
			led1_on += 2;
		led_cnt = led1_on * 2 + 2;
	}
	else if ((--led_cnt & 1) && led1_on)
	{
		led1_on--;
		on_led1;
	}
	else
	{
		off_led1;
	}
	if (led_reset)
	{
		led_reset--;
		on_led1;
	}
}

void reset_7462(String a)
{
	uart0.println("reset_7462:" + a);
	////2x3 //reset dong ho khong cho cham the cho 20s ve man hinh cho
	// 2x4reset dong ho cho cham the, cho 20s ve man hinh cho
	// 2x5 reset dong ho cho cham the
	// 2x6 cho 10s ra 2 man hinh co ban
	//  2xx tro ve ban dau

	String s = "", serialto7462 = "!^" + a + "#";
	// add_to_serial("*esp-7462-"+serialto7462+"--*");
	// serial_out();

	while (serialto7462.length())
	{

		if (serialto7462.length() > 123)
		{
			s = serial2_out.substring(0, 124);
			serial2_out.remove(0, 124);
		}
		else if (serialto7462.length())
		{
			s = serialto7462;
			serialto7462 = "";
			ms5 = (s.length() & 0x7c) + 4;
			while (s.length() < ms5)
			{
				s += "0";
			}
			ms5 = 0;
		}
		if (s.length())
		{
			Serial2.print(s);
			ms5 = 4;
		}
		delay(5);
	}
}

void bipok1()
{
	coi_on;
	delay(150);
	coi_off;
	delay(150);
}
void bipok2()
{
	bipok1();
	bipok1();
}
void biper1()
{
	coi_on;
	delay(500);
	coi_off;
	delay(500);
}
void biper2()
{
	biper1();
	biper1();
}

void Play_voice(int code)
{
	// uart0.println("play void:"+ String(code)+", play_index:"+play_index);
	if (play_index != code)
	{
		read_buf("/a_" + String(code) + ".wav");
	}
	play_index = code;
	voice_init();
}

void dongcua()
{
	bipok2();
	digitalWrite(door_gate, 1);
	Serial.println("cua dong\n");
	delay(1000);
}
void mocua()
{
	bipok1();
	digitalWrite(door_gate, 0);
	Serial.println("cua mo\n");
	delay(1000);
}

void bip4_no_internet()
{
	coi_on;
	delay(300);
	coi_off;
	delay(300);
	coi_on;
	delay(300);
	coi_off;
	delay(300);
}

void readFile(const char *path)
{
  //  Serial.printf("Reading file: %s\r\n", path);

  File file = SPIFFS.open(path);
  if (!file || file.isDirectory())
  {
    //  Serial.println("− failed to open file for reading");
    return;
  }
  ma_tbi = "";
  int n = 0;
  //  Serial.println("− read from file:");
  while (file.available())
  {
    n = file.read();
    ma_tbi += dec_to_string(n - 48);
    // Serial.write(ma_tbi);
  }
  add_to_serial(ma_tbi);
  serial_out();
  //  uart0.println(ma_tbi);
}

String dec_to_string(int a)
{
  String str = "01234567890000000ABCDEF", s = "";
  s = str.substring(a, a + 1);
  return s;
}
