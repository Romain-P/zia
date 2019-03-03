/*
** EPITECH PROJECT, 2019
** ZiaModuleAPISpec
** File description:
** Hook result spec
*/

#pragma once

namespace Zia {
namespace API {

/*
 * HOOK RESULT
 *
 * Used to test if a hook has been processed or not
 * In case of a HTTP error, just return the HTTP status code and the server should
 * take care of the rest
 */
using HookResultType = unsigned short;
namespace HookResult {

/*
 * OK
 *
 * Indicates that this hook was processed in this very request handler,
 * and that the server should skip all following requests handlers for this hook
 */
static constexpr HookResultType Ok = 1;

/*
 * DECLINED
 *
 * Indicates that this request handler refused to process this hook,
 * and that the server should ask the next request handler for this hook
 */
static constexpr HookResultType Declined = 0;

}
}
}
