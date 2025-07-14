#include "stdafx.h"
#include "Core/Parser/TokenRegister.h"
#include "Controller.h"
#include "Tokens/Common.h"

#include "Rules/Operators/If/IfRule.h"
#include "Rules/Operators/ForC/ForCRule.h"


void Init()
{
    Controller::Instance()->regOperatorRule(MakeIf);
    Controller::Instance()->regOperatorRule(MakeForC);


    Controller::Instance()->regItem<token::Unknown>(ItemType::TokenAndRule, -2);

    Controller::Instance()->regUnchangedTextToken(std::make_shared<Comment>(), std::make_shared<LComment>(), nullptr);

    Controller::Instance()->init();
}