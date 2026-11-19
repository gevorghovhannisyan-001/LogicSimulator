#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include "Simulator.h"
#include <string>
#include <vector>

// Օժանդակ ֆունկցիա՝ դարպասի մուտքը (inputSlot) միացնելու ընտրված աղբյուրին (sourceIndex)
static void drawSourceSelector(const char* label,
    const Gate*& targetInput,
    const std::vector<std::unique_ptr<Gate>>& gates,
    int selfIndex) {
    const char* previewText = targetInput ? targetInput->name.c_str() : "None (0 / disconnected)";

    if (ImGui::BeginCombo(label, previewText)) {
        bool isNoneSelected = (targetInput == nullptr);
        if (ImGui::Selectable("None (0 / disconnected)", isNoneSelected)) {
            targetInput = nullptr;
        }
        if (isNoneSelected) {
            ImGui::SetItemDefaultFocus();
        }

        for (int i = 0; i < static_cast<int>(gates.size()); ++i) {
            if (i == selfIndex) continue; // Խուսափում ենք ինքն իրեն միացնելուց (ցիկլից)

            bool isSelected = (targetInput == gates[i].get());
            if (ImGui::Selectable(gates[i]->name.c_str(), isSelected)) {
                targetInput = gates[i].get();
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
}

int main() {
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Logic Circuit Simulator", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // VSync՝ պրոցեսորը ավելորդ չծանրաբեռնելու համար

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    Simulator sim;
    int constCounter = 1;
    int andCounter = 1;
    int orCounter = 1;
    int notCounter = 1;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Կառավարման վահանակ (դարպասներ ավելացնելու կոճակներ)
        ImGui::Begin("Controls");
        if (ImGui::Button("+ Constant (1)")) {
            std::string name = "CONST_" + std::to_string(constCounter++);
            sim.createGate<ConstantGate>(name, true);
        }
        ImGui::SameLine();
        if (ImGui::Button("+ Constant (0)")) {
            std::string name = "CONST_" + std::to_string(constCounter++);
            sim.createGate<ConstantGate>(name, false);
        }

        if (ImGui::Button("+ AND Gate")) {
            std::string name = "AND_" + std::to_string(andCounter++);
            sim.createGate<AndGate>(name);
        }
        ImGui::SameLine();
        if (ImGui::Button("+ OR Gate")) {
            std::string name = "OR_" + std::to_string(orCounter++);
            sim.createGate<OrGate>(name);
        }
        ImGui::SameLine();
        if (ImGui::Button("+ NOT Gate")) {
            std::string name = "NOT_" + std::to_string(notCounter++);
            sim.createGate<NotGate>(name);
        }

        ImGui::Separator();
        if (ImGui::Button("Clear Circuit")) {
            sim.clear();
            constCounter = andCounter = orCounter = notCounter = 1;
        }
        ImGui::End();

        // 2. Սխեմայի պատուհան (մուտքեր, միացումներ և ելքային արժեքներ)
        ImGui::Begin("Circuit");
        const auto& gates = sim.getGates();

        for (int i = 0; i < static_cast<int>(gates.size()); ++i) {
            Gate* g = gates[i].get();
            bool outVal = g->evaluate();

            ImGui::PushID(i); // Շատ կարևոր է ImGui-ում, երբ նույն տիպի կոճակներ/combo-ներ կան ցիկլում
            ImGui::Separator();
            ImGui::Text("%s  -->  Output: %d", g->name.c_str(), outVal ? 1 : 0);

            if (auto* cGate = dynamic_cast<ConstantGate*>(g)) {
                bool val = cGate->getValue();
                if (ImGui::Checkbox("Value (1 = checked)", &val)) {
                    cGate->setValue(val);
                }
            }
            else if (auto* aGate = dynamic_cast<AndGate*>(g)) {
                drawSourceSelector("Input 1", aGate->input1, gates, i);
                drawSourceSelector("Input 2", aGate->input2, gates, i);
            }
            else if (auto* oGate = dynamic_cast<OrGate*>(g)) {
                drawSourceSelector("Input 1", oGate->input1, gates, i);
                drawSourceSelector("Input 2", oGate->input2, gates, i);
            }
            else if (auto* nGate = dynamic_cast<NotGate*>(g)) {
                drawSourceSelector("Input", nGate->input1, gates, i);
            }

            ImGui::PopID();
        }
        ImGui::End();

        // 3. Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.12f, 0.14f, 0.18f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}