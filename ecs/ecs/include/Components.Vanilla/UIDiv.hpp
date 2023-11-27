/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** UIDiv.hpp
*/

#ifndef E4DEBEF0_6844_494A_9013_F6F12F353078
#define E4DEBEF0_6844_494A_9013_F6F12F353078

#include <iostream>
#include "../Component.hpp"
#include "../IGraphicalModule.hpp"
#include "metadata.hpp"

needs(-lconfig++)
serialize class UIDiv : public ecs::Component<UIDiv> {
    public:
        GENERATE_METADATA(UIDiv)

        void Update(int entityID);
        void OnAddComponent(int entityID);

        void SetColor(graph::vec4uc color);
        void SetPosition(graph::vec2f position);
        void SetSize(graph::vec2f size);
        void SetBorder(graph::vec4uc color, int thickness);
        void setVisible(bool visible);
        void SetParentID(int parentID);

        graph::vec2f GetPosition() const;
        graph::vec2f GetSize() const;

    private:
        void Draw() const;

        /**
         * @brief Position of the div.
         *
         */
        serialize float m_x;
        serialize float m_y;

        /**
         * @brief Size of the div.
         *
         */
        serialize float m_width;
        serialize float m_height;

        /**
         * @brief Color of the div.
         *
         */
        serialize int m_r;
        serialize int m_g;
        serialize int m_b;
        serialize int m_a;

        /**
         * @brief Color of the div's border
         *
         */
        serialize int m_border_r;
        serialize int m_border_g;
        serialize int m_border_b;
        serialize int m_border_a;

        /**
         * @brief Thickness of the div's border
         *
         */
        serialize int m_border_thickness;

        /**
         * @brief Whether the div is visible or not.
         *
         */
        serialize bool m_visible = true;

        int m_parentID = 0;
};

#endif /* E4DEBEF0_6844_494A_9013_F6F12F353078 */
