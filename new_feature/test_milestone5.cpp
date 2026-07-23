#include "contracts.h"
#include "ai_service.h"
#include <iostream>
#include <string>

void Log(const std::wstring& msg) {
    std::string narrow(msg.begin(), msg.end());
    std::cout << narrow << std::endl;
}

int main() {
    Log(L"=== AI Voice Assistant Milestone 5 Test ===");

    ai::OpenRouterAIService aiService;

    ai::AIRequest req;
    req.prompt = L"Explain recursion in one sentence.";

    Log(L"Querying OpenRouter (sending prompt: \"" + req.prompt + L"\")...");
    
    // Execute request (runs WinHTTP request or mocks if key is empty)
    ai::AIResponse res = aiService.Chat(req);

    Log(L"OpenRouter Response received:");
    Log(L"--------------------------------------------------");
    Log(res.text);
    Log(L"--------------------------------------------------");

    Log(L"=== Milestone 5 Test Complete ===");
    return 0;
}
