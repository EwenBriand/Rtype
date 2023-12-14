#include "Animation.hpp"
#include "Animator.hpp"
#include "ResourceManager.hpp"
#include <string>
#include <vector>

void TestSaveAnimator()
{
    Animator animator;
    animator.AddAnimation("tests/testanim.anim");
    auto registered = animator.GetRegisteredAnimations();

    if (registered.size() != 1)
        throw std::runtime_error("Animator::GetRegisteredAnimations() failed, zero");
    if (registered[0] != "tests/testanim.anim")
        throw std::runtime_error("Animator::GetRegisteredAnimations() failed, bad path");

    animator.Save("testsaveanim.bin");
    Animator animator2;
    animator2.Load("testsaveanim.bin");
    registered = animator2.GetRegisteredAnimations();

    if (registered.size() != 1)
        throw std::runtime_error("Animator::GetRegisteredAnimations() failed, loaded: zero");
    if (registered[0] != "tests/testanim.anim")
        throw std::runtime_error("Animator::GetRegisteredAnimations() failed, loaded: bad path");
}

int main()
{
    TestSaveAnimator();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}