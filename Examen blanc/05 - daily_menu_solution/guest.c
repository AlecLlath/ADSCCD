#include <curl/curl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHAR_OBFUSCATION(c) ((c) ^ ((char) 42))
#define CO(c) CHAR_OBFUSCATION(c)

static inline char *string_obfuscation(char *s)
{
    size_t i;

    for (i = 0; i < strlen(s); i++) {
        s[i] = s[i] ^ ((char) 42);
    }

    return s;
}

int main(int argc, char **argv)
{
    CURL *easy_handle;
    CURLcode rc = 0;
    char *daily_menu;
    size_t daily_menu_size;
    FILE *daily_menu_stream;
    char menu_string[] = {CO('1'), CO('2'), CO('7'), CO('.'), CO('0'), CO('.'), CO('0'), CO('.'), CO('1'), CO(':'), CO('5'), CO('0'), CO('0'), CO('0'), CO('/'), CO('m'), CO('e'), CO('n'), CO('u'), CO('/'), CO('t'), CO('u'), CO('e'), CO('s'), CO('d'), CO('a'), CO('y'), '\0'};
    char dish_string[] = {CO('p'), CO('i'), CO('z'), CO('z'), CO('a'), '\0'};
    char vote_string[] = {CO('1'), CO('2'), CO('7'), CO('.'), CO('0'), CO('.'), CO('0'), CO('.'), CO('1'), CO(':'), CO('5'), CO('0'), CO('0'), CO('0'), CO('/'), CO('r'), CO('a'), CO('t'), CO('i'), CO('n'), CO('g'), CO('/'), CO('b'), CO('u'), CO('r'), CO('g'), CO('e'), CO('r'), CO('/'), CO('3'), CO('3'), '\0'};

    printf("%s\n", menu_string);

    rc = curl_global_init(CURL_GLOBAL_ALL);
    if (rc) {
        fprintf(stderr,
                "Guest error on curl global init: %s\n", curl_easy_strerror(rc));
        exit(rc);
    }

    easy_handle = curl_easy_init();
    if (!easy_handle) {
        fprintf(stderr, "Guest error when init easy_handle\n");
        rc = CURLE_FAILED_INIT; goto global_clean;
    }

    rc = curl_easy_setopt(easy_handle, CURLOPT_URL, string_obfuscation(menu_string));
    if (rc) {
        fprintf(stderr,
                "Guest error on setting menu CURLOPT_URL %s : %d, %s\n",
                menu_string, rc, curl_easy_strerror(rc));
        goto easy_clean;
    }
    printf("%s\n", menu_string);

    daily_menu_stream = open_memstream(&daily_menu, &daily_menu_size);
    if (!daily_menu_stream) {
        rc = errno;
        fprintf(stderr, "Guest error on creating dayli menu stream : %d, %s\n",
                rc, strerror(rc));
	goto easy_clean;
    }

    rc = curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, daily_menu_stream);
    if (rc) {
        fprintf(stderr,
                "Guest error on setting daily_menu_stream writedata: %d, %s\n",
                rc, curl_easy_strerror(rc));
        goto easy_clean;
    }

    rc = curl_easy_perform(easy_handle);
    if (rc) {
        fprintf(stderr,
                "Guest error on performing download: %s\n",
                curl_easy_strerror(rc));
        goto easy_clean;
    }

    fclose(daily_menu_stream);

    /* checking pizza is returned */
    if (strcmp(string_obfuscation(dish_string), daily_menu)) {
        printf("Guest did not received the expected dish for the asked day. Guest received \"%s\".\n", daily_menu);
	free(daily_menu); daily_menu = NULL;
	goto easy_clean;
    }

    free(daily_menu);

    rc = curl_easy_setopt(easy_handle, CURLOPT_URL, string_obfuscation(vote_string));
    if (rc) {
        fprintf(stderr,
                "Guest error on setting rating CURLOPT_URL %s : %d, %s\n",
                vote_string, rc, curl_easy_strerror(rc));
        goto easy_clean;
    }

    rc = curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, stdout);
    if (rc) {
        fprintf(stderr,
                "Guest error on setting back writedata: %d, %s\n",
                rc, curl_easy_strerror(rc));
        goto easy_clean;
    }

    rc = curl_easy_perform(easy_handle);
    if (rc) {
        fprintf(stderr,
                "Guest error on performing burger rating: %s\n",
                curl_easy_strerror(rc));
        goto easy_clean;
    }

easy_clean:
    curl_easy_cleanup(easy_handle);
global_clean:
    curl_global_cleanup();
    exit(rc);
}
