#include "IGame.hpp"

namespace eng {
    class Engine;
    class RType : public eng::IGame {
    public:
        RType() = default;
        ~RType() = default;

        void Init(Engine*) override;
        void Cleanup(Engine*) override;
        bool IsOnLine(Engine*) override;
        void WaitConnect(Engine*) override;
        void LoadFirstScene(Engine*) override;
    };
} // namespace eng