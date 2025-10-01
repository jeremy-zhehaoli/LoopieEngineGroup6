#include "CreateProjectModule.h"


#include "Loopie/Core/Application.h"
#include "Loopie/Core/Log.h"
#include "Loopie/Files/Json.h"

namespace Loopie {
	class EditorApplication : public Application {
	public:
		EditorApplication() : Application() {
			//// Open first the module -> Project CreationSetUp
			//// Once the Porject is selected, remove the module and add the EditorModule

			AddModule(new CreateProjectModule());

			std::string json_str = R"({"name": "Jane Doe", "age": 25})";
			JsonData data = Json::ReadFromString(json_str);

			Log::Info("{0}",data.Get<int>("age").Result);
		}
	};
}

int main() {
	Loopie::Application* app = new Loopie::EditorApplication();
	app->Run();
	delete app;
}