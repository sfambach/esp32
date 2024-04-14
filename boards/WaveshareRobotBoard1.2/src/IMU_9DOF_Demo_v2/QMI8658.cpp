#include "QMI8658.h"

#include <Wire.h>
#include <math.h>

#define QMI8658_UINT_MG_DPS
//#define M_PI			(3.14159265358979323846f)
#define ONE_G			(9.807f)


static qmi8658_state g_imu;


void QMI8658::write_reg(uint8_t reg,uint8_t value)
{
  Wire.beginTransmission(QMI8658_ADDR);
  Wire.write(reg);
  Wire.write(value);
  last_status = Wire.endTransmission();
}

uint8_t QMI8658::read_reg(uint8_t reg)
{
	uint8_t ret=0;
	unsigned int retry = 0;

	while((!ret) && (retry++ < 5))
	{
    Wire.beginTransmission(QMI8658_ADDR);
    Wire.write(reg);
    last_status = Wire.endTransmission();
    Wire.requestFrom(QMI8658_ADDR, 1);
    ret = Wire.read();
    Wire.endTransmission();
	}
	return ret;
}

uint16_t QMI8658::readWord_reg(uint8_t reg)
{
	uint8_t retH=0;
  uint8_t retL=0;

  Wire.beginTransmission(QMI8658_ADDR);
  Wire.write(reg);
  last_status = Wire.endTransmission();
  Wire.requestFrom(QMI8658_ADDR, 2);
  retL = Wire.read();
  retH = Wire.read();
  Wire.endTransmission();

	return ((retH << 8) | retL);
}


bool QMI8658::GetEulerAngles(float *pitch,float *roll, float *yaw,float acc[3],float gyro[3])
{
  read_xyz(acc,gyro);

  EulerAngles ea = get_euler_angles(gyro[0], gyro[1],gyro[2], acc[0], acc[1], acc[2]);
  *pitch = ea.pitch;
  *roll = ea.roll;
  *yaw = ea.yaw;
  return true;
}



void QMI8658::read_sensor_data(float acc[3], float gyro[3])
{
	unsigned char	buf_reg[12];
	short 			raw_acc_xyz[3];
	short 			raw_gyro_xyz[3];

	raw_acc_xyz[0] = (short)((unsigned short)( readWord_reg(Qmi8658Register_Ax_L) ));
	raw_acc_xyz[1] = (short)((unsigned short)( readWord_reg(Qmi8658Register_Ay_L) ));
	raw_acc_xyz[2] = (short)((unsigned short)( readWord_reg(Qmi8658Register_Az_L) ));

	raw_gyro_xyz[0] = (short)((unsigned short)( readWord_reg(Qmi8658Register_Gx_L) ));
	raw_gyro_xyz[1] = (short)((unsigned short)( readWord_reg(Qmi8658Register_Gy_L) ));
	raw_gyro_xyz[2] = (short)((unsigned short)( readWord_reg(Qmi8658Register_Gz_L) ));

#if defined(QMI8658_UINT_MG_DPS)
	// mg
  Serial.println("mg");
	acc[0] = (float)(raw_acc_xyz[0]*1000.0f)/g_imu.ssvt_a - TempAcc.X_Off_Err;
	acc[1] = (float)(raw_acc_xyz[1]*1000.0f)/g_imu.ssvt_a - TempAcc.Y_Off_Err;
	acc[2] = (float)(raw_acc_xyz[2]*1000.0f)/g_imu.ssvt_a - TempAcc.Z_Off_Err;
#else
	// m/s2
  Serial.println("m/s2");
	acc[0] = (float)(raw_acc_xyz[0]*ONE_G)/g_imu.ssvt_a;
	acc[1] = (float)(raw_acc_xyz[1]*ONE_G)/g_imu.ssvt_a;
	acc[2] = (float)(raw_acc_xyz[2]*ONE_G)/g_imu.ssvt_a;
#endif

#if defined(QMI8658_UINT_MG_DPS)
	// dps
  Serial.println("dps");
	gyro[0] = (float)(raw_gyro_xyz[0]*1.0f)/g_imu.ssvt_g - TempGyr.X_Off_Err;
	gyro[1] = (float)(raw_gyro_xyz[1]*1.0f)/g_imu.ssvt_g - TempGyr.Y_Off_Err;
	gyro[2] = (float)(raw_gyro_xyz[2]*1.0f)/g_imu.ssvt_g - TempGyr.Z_Off_Err;
#else
	// rad/s
  Serial.println("rad/s");
	gyro[0] = (float)(raw_gyro_xyz[0]*M_PI)/(g_imu.ssvt_g*180);		// *pi/180
	gyro[1] = (float)(raw_gyro_xyz[1]*M_PI)/(g_imu.ssvt_g*180);
	gyro[2] = (float)(raw_gyro_xyz[2]*M_PI)/(g_imu.ssvt_g*180);
#endif
}

void QMI8658::read_acc(float acc[3])
{
	unsigned char	buf_reg[12];
	short 		raw_acc_xyz[3];

	raw_acc_xyz[0] = (short)((unsigned short)( readWord_reg(Qmi8658Register_Ax_L) ));
	raw_acc_xyz[1] = (short)((unsigned short)( readWord_reg(Qmi8658Register_Ay_L) ));
	raw_acc_xyz[2] = (short)((unsigned short)( readWord_reg(Qmi8658Register_Az_L) ));

#if defined(QMI8658_UINT_MG_DPS)
	// mg
	acc[0] = (float)(raw_acc_xyz[0]*1000.0f)/g_imu.ssvt_a;
	acc[1] = (float)(raw_acc_xyz[1]*1000.0f)/g_imu.ssvt_a;
	acc[2] = (float)(raw_acc_xyz[2]*1000.0f)/g_imu.ssvt_a;
#else
	// m/s2
  Serial.println("m/s2");
	acc[0] = (float)(raw_acc_xyz[0]*ONE_G)/g_imu.ssvt_a;
	acc[1] = (float)(raw_acc_xyz[1]*ONE_G)/g_imu.ssvt_a;
	acc[2] = (float)(raw_acc_xyz[2]*ONE_G)/g_imu.ssvt_a;
#endif
}

void QMI8658::read_gyro(float gyro[3])
{
	unsigned char	buf_reg[12];
	short 		raw_gyro_xyz[3];

	raw_gyro_xyz[0] = (short)((unsigned short)( readWord_reg(Qmi8658Register_Gx_L) ));
	raw_gyro_xyz[1] = (short)((unsigned short)( readWord_reg(Qmi8658Register_Gy_L) ));
	raw_gyro_xyz[2] = (short)((unsigned short)( readWord_reg(Qmi8658Register_Gz_L) ));

#if defined(QMI8658_UINT_MG_DPS)
	// dps
	gyro[0] = (float)(raw_gyro_xyz[0]*1.0f)/g_imu.ssvt_g;
	gyro[1] = (float)(raw_gyro_xyz[1]*1.0f)/g_imu.ssvt_g;
	gyro[2] = (float)(raw_gyro_xyz[2]*1.0f)/g_imu.ssvt_g;
#else
	// rad/s
  Serial.println("rad/s");
	gyro[0] = (float)(raw_gyro_xyz[0]*M_PI)/(g_imu.ssvt_g*180);		// *pi/180
	gyro[1] = (float)(raw_gyro_xyz[1]*M_PI)/(g_imu.ssvt_g*180);
	gyro[2] = (float)(raw_gyro_xyz[2]*M_PI)/(g_imu.ssvt_g*180);
#endif
}



void QMI8658::axis_convert(float data_a[3], float data_g[3], int layout)
{
	float raw[3],raw_g[3];

	raw[0] = data_a[0];
	raw[1] = data_a[1];
	//raw[2] = data[2];
	raw_g[0] = data_g[0];
	raw_g[1] = data_g[1];
	//raw_g[2] = data_g[2];

	if(layout >=4 && layout <= 7)
	{
		data_a[2] = -data_a[2];
		data_g[2] = -data_g[2];
	}

	if(layout%2)
	{
		data_a[0] = raw[1];
		data_a[1] = raw[0];

		data_g[0] = raw_g[1];
		data_g[1] = raw_g[0];
	}
	else
	{
		data_a[0] = raw[0];
		data_a[1] = raw[1];

		data_g[0] = raw_g[0];
		data_g[1] = raw_g[1];
	}

	if((layout==1)||(layout==2)||(layout==4)||(layout==7))
	{
		data_a[0] = -data_a[0];
		data_g[0] = -data_g[0];
	}
	if((layout==2)||(layout==3)||(layout==6)||(layout==7))
	{
		data_a[1] = -data_a[1];
		data_g[1] = -data_g[1];
	}
}



void QMI8658::read_xyz(float acc[3], float gyro[3])
{
	unsigned char	status;
	unsigned char data_ready = 0;

#if defined(QMI8658_SYNC_SAMPLE_MODE)
	qmi8658_read_reg(Qmi8658Register_StatusInt, &status, 1);
	if(status&0x01)
	{
		data_ready = 1;
		qmi8658_delay_us(6);	// delay 6us
	}
#else
	status = read_reg(Qmi8658Register_Status0);
	if(status&0x03)
	{
		data_ready = 1;
	}
#endif
	if(data_ready)
	{
		// read_sensor_data(acc, gyro);
		axis_convert(acc, gyro, 0);
#if defined(QMI8658_USE_CALI)
		qmi8658_data_cali(1, acc);
		qmi8658_data_cali(2, gyro);
#endif
		g_imu.imu[0] = acc[0];
		g_imu.imu[1] = acc[1];
		g_imu.imu[2] = acc[2];
		g_imu.imu[3] = gyro[0];
		g_imu.imu[4] = gyro[1];
		g_imu.imu[5] = gyro[2];
	}
	else
	{
		acc[0] = g_imu.imu[0];
		acc[1] = g_imu.imu[1];
		acc[2] = g_imu.imu[2];
		gyro[0] = g_imu.imu[3];
		gyro[1] = g_imu.imu[4];
		gyro[2] = g_imu.imu[5];
		Serial.print("data ready fail!\n");
	}
}



void QMI8658::config_acc(enum qmi8658_AccRange range, enum qmi8658_AccOdr odr, enum qmi8658_LpfConfig lpfEnable, enum qmi8658_StConfig stEnable)
{
	unsigned char ctl_dada;

	switch(range)
	{
		case Qmi8658AccRange_2g:
			g_imu.ssvt_a = (1<<14);
			break;
		case Qmi8658AccRange_4g:
			g_imu.ssvt_a = (1<<13);
			break;
		case Qmi8658AccRange_8g:
			g_imu.ssvt_a = (1<<12);
			break;
		case Qmi8658AccRange_16g:
			g_imu.ssvt_a = (1<<11);
			break;
		default:
			range = Qmi8658AccRange_8g;
			g_imu.ssvt_a = (1<<12);
	}
	if(stEnable == Qmi8658St_Enable)
		ctl_dada = (unsigned char)range|(unsigned char)odr|0x80;
	else
		ctl_dada = (unsigned char)range|(unsigned char)odr;

	write_reg(Qmi8658Register_Ctrl2, ctl_dada);
// set LPF & HPF
	ctl_dada = read_reg(Qmi8658Register_Ctrl5);
	ctl_dada &= 0xf0;
	if(lpfEnable == Qmi8658Lpf_Enable)
	{
		ctl_dada |= A_LSP_MODE_3;
		ctl_dada |= 0x01;
	}
	else
	{
		ctl_dada &= ~0x01;
	}
	//ctl_dada = 0x00;
	write_reg(Qmi8658Register_Ctrl5,ctl_dada);
// set LPF & HPF
}

void QMI8658::config_gyro(enum qmi8658_GyrRange range, enum qmi8658_GyrOdr odr, enum qmi8658_LpfConfig lpfEnable, enum qmi8658_StConfig stEnable)
{
	// Set the CTRL3 register to configure dynamic range and ODR
	unsigned char ctl_dada;

	// Store the scale factor for use when processing raw data
	switch (range)
	{
		case Qmi8658GyrRange_16dps:
			g_imu.ssvt_g = 2048;
			break;
		case Qmi8658GyrRange_32dps:
			g_imu.ssvt_g = 1024;
			break;
		case Qmi8658GyrRange_64dps:
			g_imu.ssvt_g = 512;
			break;
		case Qmi8658GyrRange_128dps:
			g_imu.ssvt_g = 256;
			break;
		case Qmi8658GyrRange_256dps:
			g_imu.ssvt_g = 128;
			break;
		case Qmi8658GyrRange_512dps:
			g_imu.ssvt_g = 64;
			break;
		case Qmi8658GyrRange_1024dps:
			g_imu.ssvt_g = 32;
			break;
		case Qmi8658GyrRange_2048dps:
			g_imu.ssvt_g = 16;
			break;
//		case Qmi8658GyrRange_4096dps:
//			g_imu.ssvt_g = 8;
//			break;
		default:
			range = Qmi8658GyrRange_512dps;
			g_imu.ssvt_g = 64;
			break;
	}

	if(stEnable == Qmi8658St_Enable)
		ctl_dada = (unsigned char)range|(unsigned char)odr|0x80;
	else
		ctl_dada = (unsigned char)range | (unsigned char)odr;
	write_reg(Qmi8658Register_Ctrl3, ctl_dada);

// Conversion from degrees/s to rad/s if necessary
// set LPF & HPF
	ctl_dada = read_reg(Qmi8658Register_Ctrl5);
	ctl_dada &= 0x0f;
	if(lpfEnable == Qmi8658Lpf_Enable)
	{
		ctl_dada |= G_LSP_MODE_3;
		ctl_dada |= 0x10;
	}
	else
	{
		ctl_dada &= ~0x10;
	}
	//ctl_dada = 0x00;
	write_reg(Qmi8658Register_Ctrl5,ctl_dada);
// set LPF & HPF
}

void QMI8658::enableSensors(unsigned char enableFlags)
{
#if defined(QMI8658_SYNC_SAMPLE_MODE)
	qmi8658_write_reg(Qmi8658Register_Ctrl7, enableFlags | 0x80);
#elif defined(QMI8658_USE_FIFO)
	//qmi8658_write_reg(Qmi8658Register_Ctrl7, enableFlags|QMI8658_DRDY_DISABLE);
	write_reg(Qmi8658Register_Ctrl7, enableFlags);
#else
	qmi8658_write_reg(Qmi8658Register_Ctrl7, enableFlags);
#endif
	g_imu.cfg.enSensors = enableFlags&0x03;

	delay(1);
}

void QMI8658::config_reg(unsigned char low_power)
{
	enableSensors(QMI8658_DISABLE_ALL);
	if(low_power)
	{
		g_imu.cfg.enSensors = QMI8658_ACC_ENABLE;
		g_imu.cfg.accRange = Qmi8658AccRange_8g;
		g_imu.cfg.accOdr = Qmi8658AccOdr_LowPower_21Hz;
		g_imu.cfg.gyrRange = Qmi8658GyrRange_1024dps;
		g_imu.cfg.gyrOdr = Qmi8658GyrOdr_250Hz;
	}
	else
	{
		g_imu.cfg.enSensors = QMI8658_ACCGYR_ENABLE;
		g_imu.cfg.accRange = Qmi8658AccRange_16g;
		g_imu.cfg.accOdr = Qmi8658AccOdr_1000Hz;
		g_imu.cfg.gyrRange = Qmi8658GyrRange_2048dps;
		g_imu.cfg.gyrOdr = Qmi8658GyrOdr_1000Hz;
	}

	if(g_imu.cfg.enSensors & QMI8658_ACC_ENABLE)
	{
		config_acc(g_imu.cfg.accRange, g_imu.cfg.accOdr, Qmi8658Lpf_Disable, Qmi8658St_Disable);
	}
	if(g_imu.cfg.enSensors & QMI8658_GYR_ENABLE)
	{
		config_gyro(g_imu.cfg.gyrRange, g_imu.cfg.gyrOdr, Qmi8658Lpf_Disable, Qmi8658St_Disable);
	}
}

unsigned char QMI8658::get_id(void)
{
	unsigned char qmi8658_chip_id = 0x00;
	unsigned char qmi8658_revision_id = 0x00;
	unsigned char qmi8658_slave[2] = {QMI8658_SLAVE_ADDR_L, QMI8658_SLAVE_ADDR_H};
	int retry = 0;
	unsigned char iCount = 0;
	unsigned char firmware_id[3];
	unsigned char uuid[6];
	unsigned int uuid_low, uuid_high;

	while(iCount<2)
	{
		g_imu.slave = qmi8658_slave[iCount];
		retry = 0;
		while((qmi8658_chip_id != 0x05)&&(retry++ < 5))
		{
			qmi8658_chip_id = read_reg(Qmi8658Register_WhoAmI);
			Serial.printf("Qmi8658Register_WhoAmI = 0x%x\n", qmi8658_chip_id);
		}
		if(qmi8658_chip_id == 0x05)
		{
			qmi8658_on_demand_cali();

			g_imu.cfg.ctrl8_value = 0xc0;
			//QMI8658_INT1_ENABLE, QMI8658_INT2_ENABLE
			write_reg(Qmi8658Register_Ctrl1, 0x60|QMI8658_INT2_ENABLE|QMI8658_INT1_ENABLE);
			qmi8658_revision_id = read_reg(Qmi8658Register_Revision);
			// qmi8658_read_reg(Qmi8658Register_firmware_id, firmware_id, 3);
			// qmi8658_read_reg(Qmi8658Register_uuid, uuid, 6);
			write_reg(Qmi8658Register_Ctrl7, 0x00);
			write_reg(Qmi8658Register_Ctrl8, g_imu.cfg.ctrl8_value);
			// uuid_low = (unsigned int)((unsigned int)(uuid[2]<<16)|(unsigned int)(uuid[1]<<8)|(uuid[0]));
			// uuid_high = (unsigned int)((unsigned int)(uuid[5]<<16)|(unsigned int)(uuid[4]<<8)|(uuid[3]));
			// qmi8658_log("qmi8658_init slave=0x%x Revision=0x%x\n", g_imu.slave, qmi8658_revision_id);
			// qmi8658_log("Firmware ID[0x%x 0x%x 0x%x]\n", firmware_id[2], firmware_id[1],firmware_id[0]);
			// qmi8658_log("UUID[0x%x %x]\n", uuid_high ,uuid_low);
			break;
		}
		iCount++;
	}

	return qmi8658_chip_id;
}


void QMI8658::qmi8658_on_demand_cali(void)
{
	Serial.print("qmi8658_on_demand_cali start\n");
	write_reg(Qmi8658Register_Reset, 0xb0);
	delay(10);	// delay
	write_reg(Qmi8658Register_Ctrl9, (unsigned char)qmi8658_Ctrl9_Cmd_On_Demand_Cali);
	delay(2200);	// delay 2000ms above
	write_reg(Qmi8658Register_Ctrl9, (unsigned char)qmi8658_Ctrl9_Cmd_NOP);
	delay(100);	// delay
	Serial.print("qmi8658_on_demand_cali done\n");
}

unsigned char QMI8658::begin(void)
{
	if(get_id() == 0x05)
	{
#if defined(QMI8658_USE_AMD)
		qmi8658_config_amd();
#endif
#if defined(QMI8658_USE_PEDOMETER)
		qmi8658_config_pedometer(125);
		qmi8658_enable_pedometer(1);
#endif
		config_reg(0);
		enableSensors(g_imu.cfg.enSensors);
    Serial.println("Position your ICM20948 flat and don't move it - calibrating...");
    delay(1000);
    autoOffsets();
    // Serial.print();
		dump_reg();
#if defined(QMI8658_USE_CALI)
		memset(&g_cali, 0, sizeof(g_cali));
#endif
		return 1;
	}
	else
	{
		// Serial.print("qmi8658_init fail\n");
		return 0;
	}
}


void QMI8658::dump_reg(void)
{
	// unsigned char read_data[8];

	// qmi8658_read_reg(Qmi8658Register_Ctrl1, read_data, 8);
	// qmi8658_log("Ctrl1[0x%x]\nCtrl2[0x%x]\nCtrl3[0x%x]\nCtrl4[0x%x]\nCtrl5[0x%x]\nCtrl6[0x%x]\nCtrl7[0x%x]\nCtrl8[0x%x]\n",
	// 				read_data[0],read_data[1],read_data[2],read_data[3],read_data[4],read_data[5],read_data[6],read_data[7]);
}

void QMI8658::autoOffsets(void){
    float acc[3],gyro[3];

    for(int i=0; i<50; i++){
        QMI8658::read_acc(acc);
        TempAcc.X_Off_Err += acc[0];
        TempAcc.Y_Off_Err += acc[1];
        TempAcc.Z_Off_Err += acc[2];
        delay(10);
    }
    
    TempAcc.X_Off_Err /= 50;
    TempAcc.Y_Off_Err /= 50;
    TempAcc.Z_Off_Err /= 50;
    TempAcc.Z_Off_Err -= 980.0;
    
    for(int i=0; i<50; i++){
        QMI8658::read_gyro(gyro);
        TempGyr.X_Off_Err += gyro[0];
        TempGyr.Y_Off_Err += gyro[1];
        TempGyr.Z_Off_Err += gyro[2];
        delay(1);
    }
    
    TempGyr.X_Off_Err /= 50;
    TempGyr.Y_Off_Err /= 50;
    TempGyr.Z_Off_Err /= 50;
    
}

