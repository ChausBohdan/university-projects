#pragma once
#include "stdafx.h"
#include "Controller.h"

#include "Rules/IdentRule/Undefined.h"
#include "Tokens/Common/Unknown.h"

void Init();

template <typename T>
bool CheckSemantic(std::ostream& out, std::list<std::shared_ptr<T>>& tokens)
{
    auto endOfFileType = tokens.back()->type();
    std::list<std::shared_ptr<IBackusRule>> rules;
    for (auto token : tokens)
    {
        if (auto rule = std::dynamic_pointer_cast<IBackusRule>(token))
            rules.push_back(rule);
    }

    auto it = rules.begin();
    auto end = rules.end();
    std::multimap<int, std::pair<std::string, std::vector<std::string>>> errors;
    auto res = Controller::Instance()->topRule()->check(errors, it, end);

    rules.erase(++std::find_if(it, rules.end(), [&endOfFileType](const auto& rule) { return rule->type() == endOfFileType; }), rules.end());
    end = --rules.end();

    std::multimap<int, std::string> errorsMsg;

    tokens.clear();
    for (auto rule : rules)
    {
        tokens.push_back(std::dynamic_pointer_cast<T>(rule));
        if (rule->type() == Undefined::Type())
        {
            res = false;
            std::string err;
            if (auto erMsg = rule->customData("error"); !erMsg.empty())
            {
                err = "Semantic error: " + erMsg;
            }
            else
            {
                err = std::format("Semantic error: Undefined token: {}", rule->value());
            }
            errorsMsg.emplace(rule->line(), err);
        }
        else if (rule->type() == token::Unknown::Type())
        {
            res = false;
            errorsMsg.emplace(rule->line(), std::format("Lexical error: Unknown token: {}", rule->value()));
        }
    }

    for (auto it = errors.rbegin(); it != errors.rend(); ++it)
    {
        auto types = it->second.second;
        std::stringstream ss;
        for (size_t i = 0; i < types.size(); ++i)
        {
            if (!types[i].empty())
            {
                ss << types[i];

                if (i != types.size() - 1)
                    ss << " or ";
            }
        }

        auto ssStr = ss.str();
        if (!ssStr.empty())
        {

            std::string msg = "Syntax error: Expected: " + ssStr;

            if (!it->second.first.empty())
                msg += " before " + it->second.first;

            errorsMsg.emplace(it->first, msg);
        }
    }

    out << "List of errors" << std::endl;
    out << "=============================================================================" << std::endl;
    out << "Count of errors - " << errorsMsg.size() << std::endl;
    for (auto const& [line, msg] : errorsMsg)
    {
        out << "Line " << line << ": " << msg << std::endl;
    }


    return res;
}