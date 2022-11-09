/* libcurl_download.c */

#include <curl/curl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    CURL *easy_handle;
    CURLcode rc = 0;

    if (argc < 2) {
        fprintf(stderr, "usage : %s url\n", argv[0]);
        exit(EINVAL);
    }

    rc = curl_global_init(CURL_GLOBAL_ALL);
    if (rc) {
        fprintf(stderr,
                "Error on curl global init: %s\n", curl_easy_strerror(rc));
        exit(rc);
    }

    easy_handle = curl_easy_init();
    if (!easy_handle) {
        fprintf(stderr, "Error when init easy_handle\n");
        rc = CURLE_FAILED_INIT; goto global_clean;
    }

    rc = curl_easy_setopt(easy_handle, CURLOPT_URL, argv[1]);
    if (rc) {
        fprintf(stderr,
                "Error on setting CURLOPT_URL %s : %d\n",
                argv[1], curl_easy_strerror(rc));
        goto easy_clean;
    }

    rc = curl_easy_perform(easy_handle);
    if (rc) {
        fprintf(stderr,
                "Error on performing download: %s\n",
                curl_easy_strerror(rc));
        goto easy_clean;
    }

easy_clean:
    curl_easy_cleanup(easy_handle);
global_clean:
    curl_global_cleanup();
    exit(rc);
}
