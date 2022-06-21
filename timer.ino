void set_pwm_val(int16 n16)
{
	if (n16 < 0)
		n16 = 0;
	if (n16 > 511)
		n16 = 511;
	ledcWrite(pwm_ch, n16);
}

void pwm_init()
{
	ledcSetup(pwm_ch, freq, pwm_bit);
	ledcAttachPin(pwm_pin, pwm_ch);
	ledcWrite(pwm_ch, zero_level);
}
void timer_init(bool on_timer)
{
	if (on_timer)
	{
		timer = timerBegin(3, 1, true); // 80 MHz
		timerAttachInterrupt(timer, &onTimer, true);
		timerAlarmWrite(timer, 40 * 125, true); // 125 us
		timerAlarmEnable(timer);
		pwm_on = 1;
	}
	else
	{
		ledcWrite(pwm_ch, zero_level);
		timerDetachInterrupt(timer);
		timerEnd(timer);
		pwm_on = 0;
		timer = NULL;
	}
}
void IRAM_ATTR onTimer()
{
	// portENTER_CRITICAL(&timerMux);
	// portEXIT_CRITICAL(&timerMux);

	if (bip_on)
	{
		set_pwm_val(sin8[--bip_on & 7] + zero_level);
		if (bip_on == 0)
			timer_init(false);
	}
	else
	{
		if (ima_buf_cnt < voice_len)
		{
			if (ima_haft == 0)
			{
				pwm_tmp = ima_buf[ima_buf_cnt];
				pwm_val = decoder(pwm_tmp >> 4);
				ima_haft = 1;
			}
			else
			{
				pwm_val = decoder(pwm_tmp);
				ima_haft = 0;
				ima_buf_cnt++;
			}
			pwm_val >>= 7;
			set_pwm_val(pwm_val + zero_level);
		}
		else
		{
			timer_init(false);
		}
	}
}
