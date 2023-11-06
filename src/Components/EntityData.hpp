/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** EntityData
*/

#ifndef EntityData_HPP_
    #define EntityData_HPP_

namespace ECS {
    struct EntityData {
        int HP;
        unsigned int DMG;
        unsigned int LVL;

        bool isAlive() {
            if (HP < 0)
                return false;
            return true;
        }
    };
}

#endif /* !EntityData_HPP_ */
