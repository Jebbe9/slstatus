

/* interval between updates (in ms) */
const unsigned int interval = 1000;

/* text to show if no value can be retrieved */
static const char unknown_str[] = "n/a";

/* maximum output string length */
#define MAXLEN 2048

/*
 * function            description                     argument (example)
 *
 * battery_perc        battery percentage              battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * battery_state       battery charging state          battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * battery_remaining   battery remaining HH:MM         battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * cpu_perc            cpu usage in percent            NULL
 * cpu_freq            cpu frequency in MHz            NULL
 * datetime            date and time                   format string (%F %T)
 * disk_free           free disk space in GB           mountpoint path (/)
 * disk_perc           disk usage in percent           mountpoint path (/)
 * disk_total          total disk space in GB          mountpoint path (/")
 * disk_used           used disk space in GB           mountpoint path (/)
 * entropy             available entropy               NULL
 * gid                 GID of current user             NULL
 * hostname            hostname                        NULL
 * ipv4                IPv4 address                    interface name (eth0)
 * ipv6                IPv6 address                    interface name (eth0)
 * kernel_release      `uname -r`                      NULL
 * keyboard_indicators caps/num lock indicators        format string (c?n?)
 *                                                     see keyboard_indicators.c
 * keymap              layout (variant) of current     NULL
 *                     keymap
 * load_avg            load average                    NULL
 * netspeed_rx         receive network speed           interface name (wlan0)
 * netspeed_tx         transfer network speed          interface name (wlan0)
 * num_files           number of files in a directory  path
 *                                                     (/home/foo/Inbox/cur)
 * ram_free            free memory in GB               NULL
 * ram_perc            memory usage in percent         NULL
 * ram_total           total memory size in GB         NULL
 * ram_used            used memory in GB               NULL
 * run_command         custom shell command            command (echo foo)
 * separator           string to echo                  NULL
 * swap_free           free swap in GB                 NULL
 * swap_perc           swap usage in percent           NULL
 * swap_total          total swap size in GB           NULL
 * swap_used           used swap in GB                 NULL
 * temp                temperature in degree celsius   sensor file
 *                                                     (/sys/class/thermal/...)
 *                                                     NULL on OpenBSD
 *                                                     thermal zone on FreeBSD
 *                                                     (tz0, tz1, etc.)
 * uid                 UID of current user             NULL
 * uptime              system uptime                   NULL
 * username            username of current user        NULL
 * vol_perc            OSS/ALSA volume in percent      mixer file (/dev/mixer)
 *                                                     NULL on OpenBSD
 * wifi_perc           WiFi signal in percent          interface name (wlan0)
 * wifi_essid          WiFi ESSID                      interface name (wlan0)
 */
static const struct arg args[] = {
	/* function format          argument */

	/* "Valmiina olevat." Tässäkin käytetään tyylii vaan grep ja sed. */
	{ cpu_perc, 	"[  %s%% ",	NULL	},
	{ ram_perc, 	"|  %s%% ",	NULL	},

	/* Checkkaa muistin ssd:ltä, perus awk settii */
	{ run_command,  "|  %s ", "df -h | awk 'NR==5 { print $4 }'" },

	/* Nää kaks kuuluu samaan "blockkiin" tavallaan */
	/* Erottaa mun explicitly lataamat kauttaviivalla niistä ja niiden "riippuvuuksista" */
	{ run_command,  "|  %s", "xbps-query -m | wc -l"},
	{ run_command,  "/%s ", "xbps-query -l | wc -l"},

	{ run_command,  "|  %s ", "amixer sget Master | awk -F\"[][]\" '/%/ { print $2 }' | head -n1"},
	{ run_command,	"|  %s ", "wpa_cli list_networks | grep CURRENT | awk '{print $2}'"},

	/* Juoksee mun vpn scriptin - perus grep if statement */
	{ run_command,  "| %s ", "dwmvpn"},

	/* Kans vamiina, eikä outputtaa samaa vaikka laittaisit noi samal taval terminaalin, esim. "$(date "+%F %T")"*/
	{ datetime, 	"|  %s", "%F %T " },
	{ uptime, 	"|  %s ] ", "uptime -p | sed 's/up//g'"},
};
