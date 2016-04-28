//
// Created by clebeaupin on 29/03/16.
//


#ifndef __LCP_CONTENT_MODULE_H__
#define __LCP_CONTENT_MODULE_H__

#include "ILicense.h"
#include "ILcpService.h"

#include <ePub3/content_module.h>

using namespace ePub3;

namespace lcp {
    class ICredentialHandler {
    public:
        virtual void decrypt(ILicense *license) = 0;
    };

    class LcpContentModule : public ContentModule
    {
    public:
        ePub3::string GetModuleName();
        void RegisterContentFilters();
        static void Register(ILcpService *const lcpService, ICredentialHandler * credentialHandler);
        async_result<ContainerPtr> ProcessFile(const ePub3::string &path, ePub3::launch policy);
        async_result<bool> ApproveUserAction(const UserAction &action);

    private:
        static ContainerPtr DecryptContainer(ContainerPtr container);
        static ILcpService *lcpService;
        static ICredentialHandler * credentialHandler;
    };
}

#endif // __LCP_CONTENT_MODULE_H__