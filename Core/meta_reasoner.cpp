#include "meta_reasoner.h"
#include <sstream>

namespace ibcs {

MetaReasoner::MetaReasoner(std::shared_ptr<ContextManager> context)
    : context_manager(context) {}

ReasoningResult MetaReasoner::infer(const std::string& Concept, ReasoningLevel level) {
    switch (level) {
        case ReasoningLevel::LINEAR:
            return linearInfer(Concept);
        case ReasoningLevel::BRANCHED:
            return branchedInfer(Concept);
        case ReasoningLevel::INTEGRATIVE:
            return integrativeInfer(Concept);
        default:
            return {"undefined", 0.0, {"No reasoning path."}};
    }
}

ReasoningResult MetaReasoner::linearInfer(const std::string& Concept) {
    auto history = context_manager->getConceptHistory(Concept);

    std::stringstream rationale;
    for (const auto& entry : history) {
        rationale << "→ " << entry.first << " (" << entry.second << ")\n";
    }

    return {
        "Likely continuation of " + Concept,
        0.6,
        {rationale.str()}
    };
}

ReasoningResult MetaReasoner::branchedInfer(const std::string& Concept) {
    auto related = context_manager->getRelatedConcepts(Concept);
    std::vector<std::string> rationale;

    for (const auto& rel : related) {
        rationale.push_back("May lead to: " + rel);
    }

    return {
        "Potential branching for " + Concept,
        0.5,
        rationale
    };
}

ReasoningResult MetaReasoner::integrativeInfer(const std::string& Concept) {
    auto deep_context = context_manager->getFullContext();
    std::vector<std::string> rationale;

    for (const auto& ctx : deep_context) {
        if (ctx.first.find(Concept) != std::string::npos)
            rationale.push_back("Contextual link: " + ctx.first);
    }

    return {
        "High-level understanding of " + Concept,
        0.85,
        rationale
    };
}

} // namespace ibcs
