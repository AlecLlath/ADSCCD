#include <curl/curl.h>
#include <errno.h>
#include <jansson.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY1 "cle1"
#define KEY2 "cle2"
#define BADKEY "cle_fictive"
#define VALUE1 "valeur1"
#define VALUE2 "valeur2"

int main(int argc, char **argv)
{
    struct curl_slist *headers = NULL;
    json_t *new_key_value_pairs_json;
    char *stream_body = NULL;
    json_error_t json_error;
    json_t *response_json;
    size_t stream_size;
    CURL *easy_handle;
    json_t *keys_json;
    char *server_url;
    char *keys_data;
    CURLcode rc = 0;
    json_t *value;
    size_t index;
    FILE *stream;
    long status;
    char *url;
    char *key;

    if (argc < 2) {
        fprintf(stderr, "usage : %s server_to_test_url\n", argv[0]);
        exit(EINVAL);
    }

    server_url = argv[1];

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

    /******************************/
    /* Put of two key/value pairs */
    /******************************/
    /* building and setting put url */
    printf("\n\nPut of %s:%s and %s:%s\n\n", KEY1, VALUE1, KEY2, VALUE2);
    if (asprintf(&url, "%s/put", server_url) < 0) {
        fprintf(stderr, "Error on creating put url.\n");
        rc = ENOMEM; goto easy_clean;
    }

    rc = curl_easy_setopt(easy_handle, CURLOPT_URL, url);
    free(url);
    if (rc) {
        fprintf(stderr,
                "Error on setting put CURLOPT_URL %s : %d\n",
                url, curl_easy_strerror(rc));
        goto easy_clean;
    }

    /* preparing as a stream json put data */
    new_key_value_pairs_json = json_pack("{s:s, s:s}",
                                         KEY1, VALUE1, KEY2, VALUE2);
    if (!new_key_value_pairs_json) {
        fprintf(stderr, "Error on creating new_key_value_pairs_json\n");
        rc = ENOMEM; goto easy_clean;
    }

    stream_body = json_dumps(new_key_value_pairs_json, JSON_COMPACT);
    json_decref(new_key_value_pairs_json);
    if (rc) {
        fprintf(stderr, "Error on dumping new_key_value_pairs_json\n");
        rc = ENOMEM; goto easy_clean;
    }

    stream_size = strlen(stream_body);
    stream = fmemopen(stream_body, stream_size, "r");
    if (!stream) {
        rc = errno;
        fprintf(stderr,
                "Error on creating put stream: %d, %s.\n",
                errno, strerror(errno));
        goto easy_clean;
    }

    /* setting easy_handle ready for PUT */
    rc = curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
    if (rc) {
        fprintf(stderr,
                "Error on setting CURLOPT_UPLOAD for put: %d, %s\n",
                rc, curl_easy_strerror(rc));
        fclose(stream); goto easy_clean;
    }

    rc = curl_easy_setopt(easy_handle, CURLOPT_READDATA, stream);
    if (rc) {
        fprintf(stderr,
                "Error on setting CURLOPT_READDATA for put: %d, %s\n",
                rc, curl_easy_strerror(rc));
        fclose(stream); goto easy_clean;
    }

    rc = curl_easy_setopt(easy_handle, CURLOPT_INFILESIZE, stream_size);
    if (rc) {
        fprintf(stderr,
                "Error on setting CURLOPT_INFILESIZE to %d for put: %d, %s\n",
                stream_size, rc, curl_easy_strerror(rc));
        fclose(stream); goto easy_clean;
    }

    /* setting application/json as Content-Type */
    headers = curl_slist_append(headers, "Content-Type: application/json");
    if (!headers) {
        fprintf(stderr, "Error on setting json content-type.\n");
        fclose(stream); rc = ENOMEM; goto easy_clean;
    }

    rc = curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);
    if (rc) {
        fprintf(stderr,
                "Error on setting json header: %d, %s\n",
                rc, curl_easy_strerror(rc));
        fclose(stream); goto easy_clean;
    }

    rc = curl_easy_perform(easy_handle);
    fclose(stream);
    if (rc) {
        fprintf(stderr,
                "Error on performing put: %d, %s\n",
                rc, curl_easy_strerror(rc));
        goto easy_clean;
    }

    /*****************/
    /* Get one value */
    /*****************/
    printf("\n\nGetting one value from key %s\n\n", KEY2);
    /* building get url */
    if (asprintf(&url, "%s/get?key=%s", server_url, KEY2) < 0) {
        fprintf(stderr, "Error on creating get url.\n");
        rc = ENOMEM; goto easy_clean;
    }

    rc = curl_easy_setopt(easy_handle, CURLOPT_URL, url);
    free(url);
    if (rc) {
        fprintf(stderr,
                "Error on setting get CURLOPT_URL %s : %d, %s\n",
                url, rc, curl_easy_strerror(rc));
        goto easy_clean;
    }

    /* setting back stdout as curl write data */
    rc = curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, stdout);
    if (rc) {
        fprintf(stderr,
                "Error on setting stdout as curl write data stream: %d, %s.\n",
                rc, curl_easy_strerror(rc));
        goto easy_clean;
    }

    /* setting back curl handle as a GET one */
    rc = curl_easy_setopt(easy_handle, CURLOPT_HTTPGET, 1L);
    if (rc) {
        fprintf(stderr,
                "Error on setting curl handle to get: %d, %s.\n",
                rc, curl_easy_strerror(rc));
        goto easy_clean;
    }

    /* cleaning "Content-Type" header */
    curl_slist_free_all(headers);
    headers = NULL;
    headers = curl_slist_append(headers, "Content-Type:");
    if (!headers) {
        fprintf(stderr, "Error on setting null Content-Type.\n");
        rc = ENOMEM; goto easy_clean;
    }

    rc = curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);
    if (rc) {
        fprintf(stderr,
                "Error on setting null Content-Type header: %d\n",
                curl_easy_strerror(rc));
        goto easy_clean;
    }

    rc = curl_easy_perform(easy_handle);
    if (rc) {
        fprintf(stderr,
                "Error on performing GET: %d, %s\n",
                rc, curl_easy_strerror(rc));
        goto easy_clean;
    }

    /***********************************************/
    /* Post an existing key an an unregistered one */
    /***********************************************/
    /* building and setting post url */
    printf("\n\nPost of keys %s and %s\n\n", KEY1, BADKEY);
    if (asprintf(&url, "%s/post", server_url) < 0) {
        fprintf(stderr, "Error on creating post url.\n");
        rc = ENOMEM; goto easy_clean;
    }

    rc = curl_easy_setopt(easy_handle, CURLOPT_URL, url);
    free(url);
    if (rc) {
        fprintf(stderr,
                "Error on setting post CURLOPT_URL %s : %d\n",
                url, curl_easy_strerror(rc));
        goto easy_clean;
    }

    /* building json as post data */
    keys_json = json_pack("[s, s]", KEY1, BADKEY);
    if (!keys_json) {
        fprintf(stderr, "Error on creating keys_json\n");
        rc = ENOMEM; goto easy_clean;
    }

    keys_data = json_dumps(keys_json, JSON_COMPACT);
    json_decref(keys_json);
    if (!keys_data) {
        fprintf(stderr, "Error on creating keys_data\n");
        rc = ENOMEM; goto easy_clean;
    }

    rc = curl_easy_setopt(easy_handle, CURLOPT_COPYPOSTFIELDS, keys_data);
    free(keys_data);
    if (rc) {
        fprintf(stderr,
                "Error on setting post json data: %d, %s\n",
                rc, curl_easy_strerror(rc));
        goto easy_clean;
    }

    /* setting application/json as Content-Type */
    headers = curl_slist_append(headers, "Content-Type: application/json");
    if (!headers) {
        fprintf(stderr, "Error on setting json content-type.\n");
        rc = ENOMEM; goto easy_clean;
    }

    rc = curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);
    if (rc) {
        fprintf(stderr,
                "Error on setting json header: %d\n",
                curl_easy_strerror(rc));
        goto easy_clean;
    }

    /* preparing response buffer */
    stream = open_memstream(&stream_body, &stream_size);
    if (!stream) {
        rc = errno;
        fprintf(stderr,
                "Error on creating post stream: %d, %s.\n",
                errno, strerror(errno));
        goto easy_clean;
    }

    rc = curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, stream);
    if (rc) {
        fprintf(stderr,
                "Error on setting post stream: %d, %s.\n",
                rc, curl_easy_strerror(rc));
        fclose(stream);
        goto easy_clean;
    }

    /* performing http request */
    rc = curl_easy_perform(easy_handle);
    fclose(stream);
    if (rc) {
        fprintf(stderr,
                "Error on performing /post: %d, %s\n",
                rc, curl_easy_strerror(rc));
        ; goto easy_clean;
    }


    /* testing response status and getting json response */
    rc = curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &status);
    if (rc) {
        fprintf(stderr,
                "Error on getting /post status: %d, %s\n",
                rc, curl_easy_strerror(rc));
        goto easy_clean;
    }

    if (status != 200) {
        fprintf(stderr,
                "Error performing post, response status is %d:\n%s",
                status, stream_body);
        rc = EBADMSG; goto easy_clean;
    }

    response_json = json_loads(stream_body, 0, &json_error);
    if (!response_json) {
        fprintf(stderr,
                "Error on decoding /post json response: %s.\n",
                json_error.text);
        rc = EBADMSG; goto easy_clean;
    }

    /* getting reference from response_json */
    if (!json_is_object(response_json)) {
        fprintf(stderr,
                "Error response json to /post is not an object");
        json_decref(response_json); rc = EBADMSG; goto easy_clean;
    }

    printf("We /post two keys, %d are registered.\n",
           json_object_size(response_json));

    json_object_foreach(response_json, key, value) {
        printf("We got key:%s, value:%s\n", key, json_string_value(value));
    }
    json_decref(response_json);

easy_clean:
    free(stream_body);
    curl_slist_free_all(headers);
    curl_easy_cleanup(easy_handle);
global_clean:
    curl_global_cleanup();
    exit(rc);
}
