/*
** EPITECH PROJECT, 2019
** ZiaModuleAPISpec
** File description:
** Various utilities
*/

#pragma once

/*
 * ZIA_API_EXPORT macro:
 * Used to export factory & recycler functions for each module.
 */
#if defined(__linux__)
    #define ZIA_API_EXPORT extern "C"
#elif defined(_WIN32) || defined(_WIN64)
    #define ZIA_API_EXPORT extern "C" __declspec(dllexport)
#endif
