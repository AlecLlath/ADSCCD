#include <curl/curl.h>
#include <errno.h>
#include <jansson.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    struct curl_slist *headers = NULL;
    char *new_piece_data = NULL;
    json_t *new_piece_json;
    json_error_t json_error;
    json_t *response_json;
    FILE *response_stream;
    char *response = NULL;
    size_t response_size;
    CURL *easy_handle;
    char *new_piece_url = "http://127.0.0.1:5000/puzzle";
    CURLcode rc = 0;
    long status;
    unsigned int piece_number;
    unsigned int x;
    unsigned int y;
    bool next_piece = false;

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

    rc = curl_easy_setopt(easy_handle, CURLOPT_URL, new_piece_url);
    if (rc) {
        fprintf(stderr,
                "Error on setting search CURLOPT_URL %s : %d, %s\n",
                new_piece_url, rc, curl_easy_strerror(rc));
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

    /* try each piece at each place */
    for (piece_number = 1; piece_number <= 100; piece_number++) {
        next_piece = false;
        printf("Try piece %u\n", piece_number);
        for (x = 1; x <= 10 && next_piece == false; x++) {
            for (y = 1; y <= 10 && next_piece == false; y++) {
                /* building json as post data */
                new_piece_json = json_pack("{s:i, s:i, s:i}",
                                           "piece", piece_number, "x_abscissa", x, "y_ordinate", y);
                if (!new_piece_json) {
                    fprintf(stderr, "Error on creating new_piece_json\n");
                    rc = ENOMEM; goto easy_clean;
                }

                new_piece_data = json_dumps(new_piece_json, JSON_COMPACT);
                json_decref(new_piece_json);
                if (!new_piece_data) {
                    fprintf(stderr, "Error on creating new_piece_data\n");
                    rc = ENOMEM; goto easy_clean;
                }
            
                rc = curl_easy_setopt(easy_handle, CURLOPT_COPYPOSTFIELDS, new_piece_data);
                free(new_piece_data);
                if (rc) {
                    fprintf(stderr,
                            "Error on setting post json data: %d, %s\n",
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
                            "Error on performing new piece request: %d, %s\n",
                            rc, curl_easy_strerror(rc));
                    free(response); goto easy_clean;
                }
            
            
                /* testing response status */
                rc = curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &status);
                if (rc) {
                    fprintf(stderr,
                            "Error on getting new piece request status: %d, %s\n",
                            rc, curl_easy_strerror(rc));
                    free(response); goto easy_clean;
                }
            
                switch (status) {
                case 200:
                    next_piece = true;
                    printf("Puzzle terminé, voici la réponse du serveur :\n%s\n", response);
		    free(response);
		    break;
                case 201:
                    free(response);
                    next_piece = true;
		    if (piece_number == 71)
                        printf("Piece 71 : x %u, y %u.\n", x, y);
                    break;
		case 202:
                    free(response);
		    break;
                default:
                    fprintf(stderr,
                            "Error on performing new piece status 200, 201 or 202 are "
                            "expected, we got %ld.\n", status);
                    free(response); goto easy_clean;
                }
	    }
	}
    }

easy_clean:
    curl_slist_free_all(headers);
    curl_easy_cleanup(easy_handle);
global_clean:
    curl_global_cleanup();
    exit(rc);
}
