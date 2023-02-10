#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <aseprite_json_export> <output_file>\n", argv[0]);
        return 1;
    }

    // Open the input file
    FILE *input_file = fopen(argv[1], "r");
    if (!input_file) {
        printf("Error: Could not open input file %s\n", argv[1]);
        return 1;
    }

    // Read the entire contents of the input file into a buffer
    char buffer[BUFFER_SIZE];
    size_t bytes_read = fread(buffer, 1, BUFFER_SIZE, input_file);
    if (bytes_read == 0) {
        printf("Error: Could not read any data from input file %s\n", argv[1]);
        return 1;
    }
    buffer[bytes_read] = '\0';

    // Parse the JSON data
    json_error_t error;
    json_t *root = json_loads(buffer, 0, &error);
    if (!root) {
        printf("Error: Could not parse JSON data: %s (line %d, column %d)\n", error.text, error.line, error.column);
        return 1;
    }

    // Extract the frames object
    json_t *frames = json_object_get(root, "frames");
    if (!frames || !json_is_object(frames)) {
        printf("Error: Could not find frames object in JSON data\n");
        return 1;
    }

    // Open the output file
    FILE *output_file = fopen(argv[2], "wb");
    if (!output_file) {
        printf("Error: Could not open output file %s\n", argv[2]);
        return 1;
    }

    // Write the frame durations to the output file
    const char *key;
    json_t *value;
    json_object_foreach(frames, key, value) {
        int duration = json_integer_value(json_object_get(value, "duration"));
        fwrite(&duration, sizeof(int), 1, output_file);
    }

    // Clean up
    fclose(input_file);
    fclose(output_file);
    json_decref(root);

    return 0;
}
