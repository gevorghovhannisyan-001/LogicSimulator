#pragma once
#include "Gate.h"
#include <vector>
#include<utility>
#include <memory>
#include<iostream>

class Simulator {
private:
    // Պահում ենք բոլոր դարպասների հիշողությունը Smart Pointer-ների միջոցով
    std::vector<std::unique_ptr<Gate>> gates;

public:
    // Ֆունկցիա՝ նոր դարպաս ստեղծելու և սիմուլյատորին ավելացնելու համար
    template<typename T, typename... Args>
    T* createGate(Args&&... args) {
        auto gate = std::make_unique<T>(std::forward<Args>(args)...);
		T* raw_ptr = gate.get(); // Ստանում ենք հում ցուցիչը
        gates.push_back(std::move(gate));
        return raw_ptr;
    }

    // Մաքրել ամբողջ սխեման
    void clear() {
        gates.clear();
    }

    // Ստանալ բոլոր դարպասների քանակը (եթե պետք գա ինտերֆեյսում)
    size_t getGateCount() const {
        return gates.size();
    }

    const std::vector<std::unique_ptr<Gate>>& getGates() const {
        return gates;
    }
};