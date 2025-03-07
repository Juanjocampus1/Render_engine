#include "../Header_Files/Camera.h"
#include "../Header_Files/ShaderClass.h"

Camera::Camera(int width, int height, glm::vec3 position) {
    Camera::width = width;
    Camera::height = height;
    Position = position;
    firstClick = true;
    lastMouseX = width / 2.0;
    lastMouseY = height / 2.0;
    zoomOffset = 0.0;
}

void Camera::UpdateMatrix(float FOVdeg, float nearPlane, float farPlane) {
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    view = glm::lookAt(Position, Position + Orientation, Up);
    projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

    cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform) {
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window) {
    // Movimiento con WASD
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Position += speed * Orientation;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Position -= speed * Orientation;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Position -= glm::normalize(glm::cross(Orientation, Up)) * speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Position += glm::normalize(glm::cross(Orientation, Up)) * speed;

    // Movimiento vertical (similar a Blender)
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        Position += Up * speed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        Position -= Up * speed;

    // Aplicar zoom con la rueda del ratón
    Position += Orientation * (float)zoomOffset * zoomSensitivity;
    zoomOffset = 0.0;

    // Detectar si Shift está presionado
    bool isShiftPressed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
    bool isMMBPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;

    if (isMMBPressed) {
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Mantener el cursor dentro de la pantalla (cursor infinito)
        if (mouseX <= 0) {
            glfwSetCursorPos(window, width - 2, mouseY);
            lastMouseX += width;
        }
        if (mouseX >= width - 1) {
            glfwSetCursorPos(window, 1, mouseY);
            lastMouseX -= width;
        }
        if (mouseY <= 0) {
            glfwSetCursorPos(window, mouseX, height - 2);
            lastMouseY += height;
        }
        if (mouseY >= height - 1) {
            glfwSetCursorPos(window, mouseX, 1);
            lastMouseY -= height;
        }

        if (!isShiftPressed) { // Solo rotar si Shift NO está presionado
            if (firstClick) {
                lastMouseX = mouseX;
                lastMouseY = mouseY;
                firstClick = false;
            }

            float rotX = sensitivity * (float)(mouseY - lastMouseY) / height;
            float rotY = sensitivity * (float)(mouseX - lastMouseX) / width;

            glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), -rotX, glm::normalize(glm::cross(Orientation, Up)));
            glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), -rotY, Up);
            Orientation = glm::vec3(rotationY * rotationX * glm::vec4(Orientation, 1.0f));

            lastMouseX = mouseX;
            lastMouseY = mouseY;
        }
        else { // Solo desplazar si Shift está presionado
            float moveX = (speed * 7.0f) * (float)(mouseX - lastMouseX) / width; // Aumentada la velocidad
            float moveY = (speed * 7.0f) * (float)(mouseY - lastMouseY) / height;

            glm::vec3 right = glm::normalize(glm::cross(Orientation, Up));
            Position -= right * moveX;
            Position += Up * moveY;

            lastMouseX = mouseX;
            lastMouseY = mouseY;
        }
    }
    else {
        firstClick = true;
    }
}

void Camera::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    camera->zoomOffset = yOffset;
}

glm::vec3 Camera::GetRayFromMouse(GLFWwindow* window) {
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    // Normalizar las coordenadas del mouse
    float x = (2.0f * mouseX) / width - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / height;
    float z = 1.0f;
    glm::vec3 rayNDS = glm::vec3(x, y, z);

    // Convertir a coordenadas de clip
    glm::vec4 rayClip = glm::vec4(rayNDS.x, rayNDS.y, -1.0, 1.0);

    // Convertir a coordenadas de vista
    glm::vec4 rayEye = glm::inverse(glm::perspective(glm::radians(FOVdeg), (float)width / height, 0.1f, 100.0f)) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0);

    // Convertir a coordenadas del mundo
    glm::vec3 rayWorld = glm::vec3(glm::inverse(glm::lookAt(Position, Position + Orientation, Up)) * rayEye);
    rayWorld = glm::normalize(rayWorld);

    return rayWorld;
}
