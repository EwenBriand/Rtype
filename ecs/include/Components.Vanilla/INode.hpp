/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** INode.hpp
*/

#ifndef D55493BF_D092_47F2_A0EF_53A2028D97FA
#define D55493BF_D092_47F2_A0EF_53A2028D97FA
#include "../Component.hpp"
namespace ecs
{
    /**
     * @brief This INode child classes are designed to work with the
     * EventGraph class. When an INode component is added loaded in the ecs,
     * it should register itself to the EventGraph class, so that the
     * EventGraph can reference to the INode and instanciate it.
     *
     * The INode child class should either be Vanilla components or AUserComponents.
     * Nodes should normally be instantiated through the EventGraph and not directly,
     * so that the EventGraph can keep track of the instances.
     *
     * The reflection system should handle the serialization of the nodes.
     * To allow this, any member of the node that should be an input pin should
     * have its name start with "input_", and any output pin should start with
     * "output_".
     *
     * Any other member of the node will be ignored by the event graph (but if
     * marked as 'serialize' fields, they will be serialized nonetheless).
     *
     * @note As components, the INode children should not have much logic in
     * the Update method, as any significat action should be done in the Execute.
     * However, the Update method will be called nonetheless everyframe.
     */
    class INode {
        public:
            /**
             * @brief Executes the script of the node.
             * It should register the fact that it has run during this frame.
             * The outputs fields of the node should be updated to reflect the
             * changes made by the script.
             *
             * @note This method should be called only once per frame.
             *
             */
            virtual void Execute(int entityID) = 0;

            /**
             * @brief Draws the node on the screen.
             *
             */
            virtual void Draw() = 0;
    };

    class ANode : public INode, public ecs::Component<ANode> {
        public:
            void OnAddComponent(int entityID);
            void Draw() override;
    };
} // namespace ecs


#endif /* D55493BF_D092_47F2_A0EF_53A2028D97FA */
