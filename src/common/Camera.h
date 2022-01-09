#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
class Camera
{
public:
	glm::mat4 getView();
	glm::vec3 getPosition();
	void update(float dt);
	Camera(GLFWwindow *window);
private:
	glm::mat4 mylookAt(glm::vec3 eye, glm::vec3 at, glm::vec3 up);

};