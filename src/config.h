#ifndef CONFIG_H
#define CONFIG_H

typedef struct
{
	float min_scale;
	float scroll_speed;
	float drag_friction;
	float scale_friction;
} config_t;

typedef config_t config;

extern config default_config; 

config config_load(const char *filepath);
void config_generate_default(const char *filepath);
void config_print(config conf, const char *filename);

#endif // CONFIG_H
