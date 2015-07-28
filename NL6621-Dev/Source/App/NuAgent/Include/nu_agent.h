#ifndef __NU_AGENT_H__
#define __NU_AGENT_H__

#define GAGENT_DID_LEN_MAX          24

/* XPG GAgent Global Config data*/
typedef struct GAGENT_CONFIG {
	unsigned int magicNumber;
	unsigned int flag;
	char wifipasscode[16]; /* gagent passcode */
	char wifi_ssid[32]; /* WiFi AP SSID */
	char wifi_key[32]; /* AP key */
}Agent_config_t;

/* GAgent_CONFIG_S flag */
#define XPG_CFG_FLAG_CONNECTED   0X00000001
#define XPG_CFG_FLAG_CHANGEPW    0X00000002 

#define WIFI_MODE_AP            (1<<0)
#define WIFI_MODE_STATION       (1<<1)
#define WIFI_MODE_ONBOARDING    (1<<2)
#define WIFI_MODE_BINDING       (1<<3)
#define WIFI_STATION_STATUS     (1<<4)


#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#endif  /*-------- __NU_AGENT_H__ --------*/
