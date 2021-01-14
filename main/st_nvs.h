#include "nvs_flash.h"
#include "nvs.h"

const char* STORAGE_NAMESPACE = "silvetech";

size_t read_nvs_key(char* key_nvs, char *value)
{
	nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    (err != ESP_OK) ? printf("erro 00"): NULL;

    //read
    size_t required_size = 0;
    err = nvs_get_str(my_handle, key_nvs, NULL, &required_size);
    (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) ? printf("erro 01"): NULL;
    //printf("\nKey %s:",key_nvs);
    if (required_size == 0) {
    	printf("Nothing saved yet!");
    } else {
    	char* var = malloc(required_size);
    	err = nvs_get_str(my_handle, key_nvs, var, &required_size);
    	if (err != ESP_OK) {
                	free(var);
    	}

    	for (int i = 0; i < required_size; i++) {
//    		printf("%c", var[i]);
    		value[i] = var[i];
    	}
    	free(var);
    }

    // Commit
    err = nvs_commit(my_handle);
    (err != ESP_OK) ? printf("erro commit nvs"):NULL;

    // Close
    nvs_close(my_handle);
    return required_size;
}

size_t read_nvs_key_size(char* key_nvs)
{
	nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    (err != ESP_OK) ? printf("erro 00"): NULL;

    //read
    size_t required_size = 0;
    err = nvs_get_str(my_handle, key_nvs, NULL, &required_size);

    // Commit
    err = nvs_commit(my_handle);
    (err != ESP_OK) ? printf("erro commit nvs"):NULL;

    // Close
    nvs_close(my_handle);
    return required_size;
}

void read_nvs_and_print_value(char* key_nvs){

	nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    (err != ESP_OK) ? printf("erro 00"): NULL;

    //read
    size_t required_size = 0;
    err = nvs_get_str(my_handle, key_nvs, NULL, &required_size);
    (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) ? printf("erro 01"): NULL;
    printf("\n %s:",key_nvs);
    if (required_size == 0) {
    	printf("Nothing saved yet!");
    } else {
    	char* var = malloc(required_size);
    	err = nvs_get_str(my_handle, key_nvs, var, &required_size);
    	if (err != ESP_OK) {
                	free(var);
    	}
    	for (int i = 0; i < required_size; i++) {
    		printf("%c", var[i]);
    	}
    	free(var);
    }

    // Commit
    err = nvs_commit(my_handle);
    (err != ESP_OK) ? printf("erro commit nvs"):NULL;

    // Close
    nvs_close(my_handle);
}


void write_nvs_key(char* key_nvs, char *value_nvs)
{
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    (err != ESP_OK) ? printf("erro 02"): NULL;

    // write
    err = nvs_set_str(my_handle, key_nvs, value_nvs);
    (err != ESP_OK) ? printf("erro 03"):printf("\nkey_value done.");

    // Commit
    err = nvs_commit(my_handle);
    (err != ESP_OK) ? printf("erro 03"):printf("\nnvs commit done.");

    // Close
    nvs_close(my_handle);
}

void start_nvs(void)
{
	esp_err_t err = nvs_flash_init();
	if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		err = nvs_flash_init();
	}
	ESP_ERROR_CHECK( err );
}

void st_init_print(void)
{
	read_nvs_and_print_value("serial_number");
	read_nvs_and_print_value("wifi_ssid");
	//read_nvs_and_print_value("wifi_pass");
	//read_nvs_and_print_value("net_ip");
	//read_nvs_and_print_value("net_mask");
	//read_nvs_and_print_value("net_gateway");
	//read_nvs_and_print_value("account_n");
	//read_nvs_and_print_value("pass_ac");
	//read_nvs_and_print_value("broker_uri");
	//read_nvs_and_print_value("broker_cert");


}
