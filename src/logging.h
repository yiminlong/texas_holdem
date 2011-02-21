
void logging_init();
void logging_shutdown();

void logging_critical(char *message, ...);
void logging_warning(char *message, ...);
void logging_info(char *message, ...);
void logging_debug(char *message, ...);

