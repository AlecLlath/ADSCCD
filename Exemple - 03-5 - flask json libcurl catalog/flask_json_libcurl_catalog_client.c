/* flask_json_libcurl_catalog_client.c */

#include <curl/curl.h>
#include <errno.h>
#include <jansson.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    struct curl_slist *headers = NULL;
    char *new_objects_data = NULL;
    json_t *new_objects_json;
    json_error_t json_error;
    json_t *response_json;
    FILE *response_stream;
    char *response = NULL;
    size_t response_size;
    char *reference_one;
    char *reference_two;
    CURL *easy_handle;
    char *server_url;
    CURLcode rc = 0;
    long status;
    char *url;

    if (argc < 2) {
        fprintf(stderr, "usage : %s server_to_test_url\n", argv[0]);
        exit(EINVAL);
    }

    server_url = argv[1];

    rc = curl_global_init(CURL_GLOBAL_ALL);
    if (rc) {
        fprintf(stderr,
                "Error on curl global init: %d, %s\n",
                rc, curl_easy_strerror(rc));
        exit(rc);
    }

    easy_handle = curl_easy_init();
    if (!easy_handle) {
        fprintf(stderr, "Error when init easy_handle\n");
        rc = CURLE_FAILED_INIT; goto global_clean;
    }

    /******************************/
    /* Search without setting ref */
    /******************************/
    printf("\n\nSearch without setting ref\n\n");
    if (asprintf(&url, "%s/search", server_url) < 0) {
        fprintf(stderr, "Error on creating search url.\n");
        rc = ENOMEM; goto easy_clean;
    }

    rc = curl_easy_setopt(easy_handle, CURLOPT_URL, url);
    free(url);
    if (rc) {
        fprintf(stderr,
                "Error on setting search CURLOPT_URL %s : %d, %s\n",
                url, rc, curl_easy_strerror(rc));
        goto easy_clean;
    }

    rc = curl_easy_perform(easy_handle);
    if (rc) {
        fprintf(stderr,
                "Error on performing url_search_no_ref: %d, %s\n",
                curl_easy_strerror(rc));
        goto easy_clean;
    }

    /******************************/
    /* Adding new objects */
    /******************************/
    printf("\n\nAdding new objects\n\n");
    /* building post url */
    if (asprintf(&url, "%s/new_references", server_url) < 0) {
        fprintf(stderr, "Error on creating adding new object url.\n");
        rc = ENOMEM; goto easy_clean;
    }

    rc = curl_easy_setopt(easy_handle, CURLOPT_URL, url);
    free(url);
    if (rc) {
        fprintf(stderr,
                "Error on setting new_reference CURLOPT_URL %s : %d, %s\n",
                url, rc, curl_easy_strerror(rc));
        goto easy_clean;
    }

    /* building json as post data */
    new_objects_json = json_pack("[{s:s, s:i, s:s}, {s:s, s:i, s:s}]",
                                 "name", "new_object_one", "price", 3,
                                 "description", "cheep fashionable object one",
                                 "name", "new_object_two", "price", 375,
                                 "description", "expensive object two");
    if (!new_objects_json) {
        fprintf(stderr, "Error on creating new_objects_json\n");
        rc = ENOMEM; goto easy_clean;
    }

    new_objects_data = json_dumps(new_objects_json, JSON_COMPACT);
    json_decref(new_objects_json);
    if (!new_objects_data) {
        fprintf(stderr, "Error on creating new_objects_data\n");
        rc = ENOMEM; goto easy_clean;
    }

    rc = curl_easy_setopt(easy_handle, CURLOPT_COPYPOSTFIELDS, new_objects_data);
    free(new_objects_data);
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
                "Error on setting json header: %d, %s\n",
                rc, curl_easy_strerror(rc));
        goto easy_clean;
    }

    /* preparing response buffer */
    response_stream = open_memstream(&response, &response_size);
    if (!response_stream) {
        rc = errno;
        fprintf(stderr,
                "Error on creating response_stream: %d, %s.\n",
                errno, strerror(errno));
        goto easy_clean;
    }

    rc = curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, response_stream);
    if (rc) {
        fprintf(stderr,
                "Error on setting response_stream: %d, %s.\n",
                rc, curl_easy_strerror(rc));
        fclose(response_stream); free(response);
        goto easy_clean;
    }

    /* performing http request */
    rc = curl_easy_perform(easy_handle);
    fclose(response_stream);
    if (rc) {
        fprintf(stderr,
                "Error on performing /new_references: %d, %s\n",
                rc, curl_easy_strerror(rc));
        free(response); goto easy_clean;
    }


    /* testing response status and getting json response */
    rc = curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &status);
    if (rc) {
        fprintf(stderr,
                "Error on getting /new_references status: %d, %s\n",
                rc, curl_easy_strerror(rc));
        free(response); goto easy_clean;
    }

    switch (status) {
    case 400:
        fprintf(stderr,
                "Error 400 on performing /new_references:\n%s",
                response);
        free(response);
        goto easy_clean;
    case 201:
        response_json = json_loads(response, 0, &json_error);
        free(response);
        if (!response_json) {
            fprintf(stderr,
                    "Error on decoding /new_references json response: %s.\n",
                    json_error.text);
            goto easy_clean;
        }
        break;
    default:
        fprintf(stderr,
                "Error on performing /new_references, status 201 or 400 are "
                "expected, we got %d.\n", status);
        free(response); goto easy_clean;
    }

    /* getting reference from response_json */
    rc = json_unpack(response_json, "[{s:s}, {s:s}]",
                     "reference", &reference_one,
                     "reference", &reference_two);
    if (rc) {
        fprintf(stderr,
                "Error on getting new references from json response.");
        json_decref(response_json); goto easy_clean;
    }

    printf("My two new objects are referenced as %s and %s .\n",
           reference_one, reference_two);

    /***************************/
    /* Getting an existing ref */
    /***************************/
    printf("\n\nGetting an existing ref\n\n");
    /* building and setting get url from reference one */
    rc = asprintf(&url, "%s/search?ref=%s", server_url, reference_one);
    json_decref(response_json);
    if (rc < 0) {
        fprintf(stderr, "Error on building search url.\n");
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
        fprintf(stderr, "Error on setting null content-type.\n");
        rc = ENOMEM; goto easy_clean;
    }

    rc = curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);
    if (rc) {
        fprintf(stderr,
                "Error on setting null Content-Type header: %d, %s\n",
                rc, curl_easy_strerror(rc));
        goto easy_clean;
    }

    rc = curl_easy_perform(easy_handle);
    if (rc) {
        fprintf(stderr,
                "Error on performing GET: %d, %s\n",
                rc, curl_easy_strerror(rc));
        goto easy_clean;
    }

easy_clean:
    curl_slist_free_all(headers);
    curl_easy_cleanup(easy_handle);
global_clean:
    curl_global_cleanup();
    exit(rc);
}
