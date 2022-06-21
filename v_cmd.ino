void new_cmd(String s0,uint8 src)
{
	//s0 = "^2x1^";	
	int k;
	String s,s1;		
	while(s0.startsWith("^"))
	{	s0.remove(0,1);//s0 =2x1^
		k=s0.indexOf("^");//k=3
		if (k >= 0)
		{	s1=s0.substring(0,k);//s1=2x1
			s0.remove(0,k);//s0=""
			k=s1.charAt(0);
			s1.remove(0,1);
			switch(k)
			{	
				case pc_to_lcd:
					if (s1.length())
			      	{	
			      	}
				case nfc_to_lcd:
			    	if (s1.length())
			      	{	
			      	}
					break;
				case wifi_reset:
  					WiFi.disconnect(true); 
				    delay(2000);
  					ESP.restart();
					break;
				case rs485_query:
					// add_to_serial(s1);
					// serial_out();
					break;
				case pc_to_nfc:
					resetServicesStatus();

					serial2_out += "!^2" + s1 + "#";
					// serial2_out="!^2xx3#";//ducmt
					// serial2_out="!^2x7#";//ducmt
					uart0.println("du_1:"+serial2_out);
					break;
				case pc_to_https:
				  	// send_https(https_encode(s1));
				  	send_https(s1);
					break;			
				case ram_write:
					ram_wr(s1);
					break;			
				case wifi_write:
					save_ssid(s1);
					break;			
				case pc_to_esp:
	
					while(s1.startsWith("*"))
					{
						s1.remove(0,1);
						k=s1.indexOf("*");
						if (k >= 0)
						{	
							s=s1.substring(0,k);
							s1.remove(0,k);
							// add_to_serial("from may tram--"+s+"--");
		 				// 	serial_out();
							caidatthietbi(s);
						}
						else
						{	s1="";
						}
					}
					break;			
			}
		}	
		else
		{	s0="";
		}
	}	
	// serial_out();
}

void resetServicesStatus()
{
	//nam
	Validate = false;
	// TrangThaiThanhToan = false;
	// magiaodich="";
	//======
	str_door = "";
	v_stat=fx_waiting;
	dataServices[0] = "";
	strdvtt="";
	str_lenhtbngoai="";
	idDelaySend[0]="";
	solannhapmk=0;
}

void ram_rd(String s)
{	for(int i=0;i<v_fx.dv_index_max;i++)
	{	if (s.length() < 3500)	s += "dv"+n_fix_len(i,2)+dv_td[i]+"_";
	}
	// add_to_serial(s);
	for(int i=0;i<v_fx.ds_index_max;i++)
	{	s += "ds"+n_fix_len(i,2)+"_";
		for (int j=0;j<v_fx.sub_ds_index_max[i];j++)
		{	if (s.length() < 3500)	s += ds_name[i][j]+"_";
		}
	}
	// add_to_serial(s);
}

void new_time(String s)
{	s.remove(0,5);
	if (s.length() > 5)
	{	ms64=millis();
		n_time[0]=n_time[3]=s.substring(0,2).toInt();
		s.remove(0,2);
		n_time[1]=n_time[4]=s.substring(0,2).toInt();
		s.remove(0,2);
		n_time[2]=n_time[5]=s.substring(0,2).toInt();
	}
}
String n_fix_len(int16 n,int16 len)
{	return fix_len(String(n),len);
}

String fix_len(String s,int16 len)
{	if (s.length() > len)	s=s.substring(s.length()-len,s.length());
	while (s.length() < len)	{	s = "0"+s;}
	return s;
}

void ram_wr(String s)
{
	String s0;	
	int k;	
	while(s.startsWith("*"))
	{	s.remove(0,1);
		k=s.indexOf("*");
		if (k >= 0)
		{	s0=s.substring(0,k);
			s.remove(0,k);
    	}
    	else
    	{	s="";
    	}
	}
	v_stat=dv_selecting;
	// lcd.clear();
	// lcd.print(String(v_fx.dv_index_max));
}
String bcc_check_len(String s,int a)
{	int32 bcc=0;	
	for (int i=0;i<s.length();i++)	{	bcc += (uint8)s.charAt(i);}
	return fix_len(String(bcc),a);
}
String bcc_check_step(String s, int step){
	int32 bcc=0;	
	int32 tong = 0;
	int i = 0;
	for (i=0;i<s.length();i++){	
		bcc = (uint8)s.charAt(i);
		if(i % step == 0) bcc = bcc + ( i / step) + 3;
		tong += bcc;
	}
	return fix_len(String(tong),4);
}

void add_to_serial(String s)
{ 
  serial_rs485 = s;
}
void serial_out()
{ 
  if (serial_rs485.length())
  { 
    rs485_tx_out=127;
    rs485_out;
    Serial.print(serial_rs485);
    serial_rs485="";
  }
}

void khoitao_ghixacthuc(String s){

	dau_dlxacthuc = s.substring(1,20);
	s.remove(0,20);
	dulieughixacthuc = s;
	s="";
	trangthailenhtumaytinh = 1;
	// reset_7462("2x7");// bat dau ghi xac thuc	
}

void Docxacthuc5(){ // doc xac thuc fi5
	 bipok1();
	reset_7462("2xx1");
}
void Docxacthuc6(){// doc xac thuc fi6
	 bipok1();
	reset_7462("2xx2");
}