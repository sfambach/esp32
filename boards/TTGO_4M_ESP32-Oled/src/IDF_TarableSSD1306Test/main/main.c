/**
 * Print text and increasing counter, counter will be set to 0 after  999
 *
 * Initial Display Version was created by Tara Keeling
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 *
 * The timer handling is from an example project /system/esp_time of the idf.
 *
 * For more informations visit http://fambach.net if you want ;) 
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "sdkconfig.h"
#include "ssd1306.h"
#include "ssd1306_draw.h"
#include "ssd1306_font.h"
#include "ssd1306_default_if.h"


static const int I2CDisplayAddress = 0x3C;
static const int I2CDisplayWidth = 128;
static const int I2CDisplayHeight = 64;     // <-- 32
static const int I2CResetPin = 16;          // <-- fill in reset
static int count = 0;
static const char* TAG = "example";

struct SSD1306_Device I2CDisplay;


void SayHello( struct SSD1306_Device* DisplayHandle, const char* HelloText , int count);
static void periodic_timer_callback(void* arg);

bool DefaultBusInit( void ) {
    assert( SSD1306_I2CMasterInitDefault( ) == true );
    assert( SSD1306_I2CMasterAttachDisplayDefault( &I2CDisplay, I2CDisplayWidth, I2CDisplayHeight, I2CDisplayAddress, I2CResetPin ) == true );
    
    return true;
}



void SayHello( struct SSD1306_Device* DisplayHandle, const char* HelloText , int count) {
	
	SSD1306_Clear( DisplayHandle, SSD_COLOR_BLACK );
	char text [3]= "";
	sprintf(text, "%d", count++);
	//itoa(count++,text,4);
    int textWidth = SSD1306_FontMeasureString( DisplayHandle, text );
    int textHeight = SSD1306_FontGetCharHeight( DisplayHandle);

    int centerY = DisplayHandle->Height / 2 - textHeight/2;
    
    SSD1306_FontDrawString( DisplayHandle, 0, centerY , HelloText, SSD_COLOR_WHITE );
    SSD1306_FontDrawString( DisplayHandle, DisplayHandle->Width - textWidth, centerY, text, SSD_COLOR_WHITE );
    SSD1306_Update( DisplayHandle );
}



void app_main( void ) {
   
   // init display
 
   if ( DefaultBusInit( ) == true ) {
		SSD1306_Clear( &I2CDisplay, SSD_COLOR_BLACK );
		SSD1306_SetVFlip( &I2CDisplay,true);
		SSD1306_SetHFlip( &I2CDisplay,true);
		SSD1306_SetFont( &I2CDisplay, &Font_droid_sans_mono_7x13);
   }
   
	// creating timer 
    const esp_timer_create_args_t periodic_timer_args = {
            .callback = &periodic_timer_callback,
            /* name is optional, but may help identify the timer when debugging */
            .name = "periodic"
    };

    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    
	
	/* Start the timers */
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 500000));
   
}


static void periodic_timer_callback(void* arg)
{
    int64_t time_since_boot = esp_timer_get_time();
    ESP_LOGI(TAG, "Periodic timer called, time since boot: %lld us", time_since_boot);
	
	SayHello( &I2CDisplay, "Stefan's Blog!", count++ );
			
	if(count > 999){
		count = 0;
	}
	
	
}

