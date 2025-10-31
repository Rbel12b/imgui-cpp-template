import os

def replace_in_file(filename, placeholder, value):
    with open(filename, "r", encoding="utf-8") as f:
        content = f.read()
    content = content.replace(placeholder, value)
    with open(filename, "w", encoding="utf-8") as f:
        f.write(content)

def main():
    project_name = input("Enter your project name: ").strip()
    if not project_name:
        print("Project name cannot be empty.")
        return

    replace_in_file("CMakeLists.txt", "{{PROJECT_NAME}}", project_name)
    replace_in_file("src/Utils.cpp", "{{PROJECT_NAME}}", project_name)
    replace_in_file("src/Renderer.cpp", "{{PROJECT_NAME}}", project_name)

    print(f"Project configured as '{project_name}'")

if __name__ == "__main__":
    main()
