#include "../include/renderer.h"

static GLuint renderer_load_shader(const char *file) {
    // create file paths for vertex and fragment shaders
    const char *folder = "./shaders/";
    const char *vertex_extension   = ".vs.glsl";
    const char *fragment_extension = ".fs.glsl";

    int vertex_shader_path_length   = (strlen(folder) + strlen(file) + strlen(vertex_extension) + 1);
    int fragment_shader_path_length = (strlen(folder) + strlen(file) + strlen(fragment_extension) + 1);
    char *vertex_shader_path   = malloc(sizeof(char) * vertex_shader_path_length);
    char *fragment_shader_path = malloc(sizeof(char) * fragment_shader_path_length);
    
    assert(vertex_shader_path != NULL);
    assert(fragment_shader_path != NULL);
    
    vertex_shader_path[0] = '\0';
    fragment_shader_path[0] = '\0';

    strcat(vertex_shader_path, folder);
    strcat(vertex_shader_path, file);
    strcat(vertex_shader_path, vertex_extension);

    strcat(fragment_shader_path, folder);
    strcat(fragment_shader_path, file);
    strcat(fragment_shader_path, fragment_extension);

    // read the vertex shader data 
    FILE *vertex_fp;
    if ((vertex_fp = fopen(vertex_shader_path, "r")) == NULL) {
        fprintf(stderr, "Error: could not open vertex shader from %s\n", vertex_shader_path);
        free(vertex_shader_path);
        free(fragment_shader_path);
        exit(EXIT_FAILURE);
    }
    free(vertex_shader_path);
    
    fseek(vertex_fp, 0, SEEK_END);
    size_t vertex_source_length = ftell(vertex_fp);
    fseek(vertex_fp, 0, SEEK_SET);

    char *vertex_source = malloc(sizeof(char) * (vertex_source_length + 1));
    assert(vertex_source != NULL);

    if (fread(vertex_source, 1, vertex_source_length, vertex_fp) != vertex_source_length) {
        fprintf(stderr, "Error: could not read vertex shader from file\n");
        free(vertex_source);
        fclose(vertex_fp);
        free(fragment_shader_path);
        exit(EXIT_FAILURE);
    }
    vertex_source[vertex_source_length] = '\0';
    fclose(vertex_fp);
    
    // read the fragment shader data
    FILE *fragment_fp;
    if ((fragment_fp = fopen(fragment_shader_path, "r")) == NULL) {
        fprintf(stderr, "Error: could not open fragment shader from %s\n", fragment_shader_path);
        free(vertex_source);
        free(fragment_shader_path);
        exit(EXIT_FAILURE);
    }
    free(fragment_shader_path);

    fseek(fragment_fp, 0, SEEK_END);
    size_t fragment_source_length = ftell(fragment_fp);
    fseek(fragment_fp, 0, SEEK_SET);

    char *fragment_source = malloc(sizeof(char) * (fragment_source_length + 1));
    assert(fragment_source != NULL);

    if (fread(fragment_source, 1, fragment_source_length, fragment_fp) != fragment_source_length) {
        fprintf(stderr, "Error: could not read fragment shader from file\n");
        free(vertex_source);
        free(fragment_source);
        fclose(fragment_fp);
        exit(EXIT_FAILURE);
    }
    fragment_source[fragment_source_length] = '\0';
    fclose(fragment_fp);

    // compile and link shaders
    char info_log[512];
    GLint result = 0;
    GLuint vertex_shader   = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    
    // compile vertex shader
    glShaderSource(vertex_shader, 1, (const GLchar **)&vertex_source, NULL);
    glCompileShader(vertex_shader);
    free(vertex_source);
    
    // check vertex shader compile status
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        fprintf(stderr, "Error: Could not compile vertex shader %s\n", info_log);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        free(fragment_source);
        exit(EXIT_FAILURE);
    }

    // compile fragment shader
    glShaderSource(fragment_shader, 1, (const GLchar **)&fragment_source, NULL);
    glCompileShader(fragment_shader);
    free(fragment_source);
   
    // check fragment shader compile status
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        fprintf(stderr, "Error: Could not compile fragment shader %s\n", info_log);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        exit(EXIT_FAILURE);
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    // check program link status
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(program, 512, NULL, info_log);
        fprintf(stderr, "Error: Could not link program %s\n", info_log);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        glDeleteProgram(program);
        exit(EXIT_FAILURE);
    }

    glDetachShader(program, vertex_shader);
    glDetachShader(program, fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    printf("[LOG]: loaded shaders\n");

    return program;
}

void renderer_init(renderer_t *r, const char *shader_file) {
    // create vertex buffers and allocate memory for verticies
    glGenBuffers(1, &r->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
    glBufferData(GL_ARRAY_BUFFER, MAX_VERTICIES * sizeof(vertex_t), NULL, GL_DYNAMIC_DRAW);

    // create vertex arrays and set the attribute locations
    glGenVertexArrays(1, &r->vao);
    glBindVertexArray(r->vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // load and compile shaders
    r->shader = renderer_load_shader(shader_file);

}

void renderer_start_frame(renderer_t *r) {
    glClear(GL_COLOR_BUFFER_BIT);
    r->triangle_count = 0;
}

void renderer_end_frame(renderer_t *r) {
    glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * r->triangle_count * sizeof(vertex_t), r->render_vertcies);

    glUseProgram(r->shader);
    glBindVertexArray(r->vao);
    glDrawArrays(GL_TRIANGLES, 0, r->triangle_count * 3);
}

void renderer_push_triangle
(
    renderer_t *r,
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    float x3, float y3, float z3,
    float r1, float b1, float g1,
    float r2, float b2, float g2,
    float r3, float b3, float g3
) 
{
    if (r->triangle_count == MAX_TRIANGLES) {
        renderer_end_frame(r);
        renderer_start_frame(r);
    }

    r->render_vertcies[r->triangle_count * 3 + 0].x = x1;
    r->render_vertcies[r->triangle_count * 3 + 0].y = y1;
    r->render_vertcies[r->triangle_count * 3 + 0].z = z1;
    r->render_vertcies[r->triangle_count * 3 + 1].x = x2;
    r->render_vertcies[r->triangle_count * 3 + 1].y = y2;
    r->render_vertcies[r->triangle_count * 3 + 1].z = z2;
    r->render_vertcies[r->triangle_count * 3 + 2].x = x3;
    r->render_vertcies[r->triangle_count * 3 + 2].y = y3;
    r->render_vertcies[r->triangle_count * 3 + 2].z = z3;

    r->render_vertcies[r->triangle_count * 3 + 0].r = r1;
    r->render_vertcies[r->triangle_count * 3 + 0].g = g1;
    r->render_vertcies[r->triangle_count * 3 + 0].b = b1;
    r->render_vertcies[r->triangle_count * 3 + 1].r = r2;
    r->render_vertcies[r->triangle_count * 3 + 1].g = g2;
    r->render_vertcies[r->triangle_count * 3 + 1].b = b2;
    r->render_vertcies[r->triangle_count * 3 + 2].r = r3;
    r->render_vertcies[r->triangle_count * 3 + 2].g = g3;
    r->render_vertcies[r->triangle_count * 3 + 2].b = b3;

    r->triangle_count++;
}
