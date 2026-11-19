#pragma once
#include <string>

class Gate {
public:
    std::string name;

    explicit Gate(std::string gateName) : name(std::move(gateName)) {}
    virtual bool evaluate() const = 0;
    virtual ~Gate() = default;
};

class ConstantGate : public Gate {
private:
    bool value;
public:
    ConstantGate(std::string gateName, bool val)
        : Gate(std::move(gateName)), value(val) {
    }

    bool evaluate() const override {
        return value;
    }

    void setValue(bool val) {
        value = val;
    }

    bool getValue() const {
        return value;
    }
};

class AndGate : public Gate {
public:
    const Gate* input1 = nullptr;
    const Gate* input2 = nullptr;

    explicit AndGate(std::string gateName) : Gate(std::move(gateName)) {}

    bool evaluate() const override {
        if (!input1 || !input2) return false;
        return input1->evaluate() && input2->evaluate();
    }
};

class OrGate : public Gate {
public:
    const Gate* input1 = nullptr;
    const Gate* input2 = nullptr;

    explicit OrGate(std::string gateName) : Gate(std::move(gateName)) {}

    bool evaluate() const override {
        bool v1 = input1 ? input1->evaluate() : false;
        bool v2 = input2 ? input2->evaluate() : false;
        return v1 || v2;
    }
};

class NotGate : public Gate {
public:
    const Gate* input1 = nullptr;

    explicit NotGate(std::string gateName) : Gate(std::move(gateName)) {}

    bool evaluate() const override {
        if (!input1) return true;
        return !input1->evaluate();
    }
};