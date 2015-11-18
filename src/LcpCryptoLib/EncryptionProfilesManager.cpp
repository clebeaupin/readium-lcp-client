#include "EncryptionProfilesManager.h"
#include "Lcp1dot0EncryptionProfile.h"

namespace lcp
{
    EncryptionProfilesManager::EncryptionProfilesManager()
    {
        std::unique_ptr<IEncryptionProfile> lcp1dot0(new Lcp1dot0EncryptionProfile());
        m_profilesMap.insert(std::make_pair(lcp1dot0->Self(), std::move(lcp1dot0)));
    }

    bool EncryptionProfilesManager::RegisterProfile(std::unique_ptr<IEncryptionProfile> profile)
    {
        auto res = m_profilesMap.insert(std::make_pair(profile->Self(), std::move(profile)));
        return res.second;
    }

    IEncryptionProfile * EncryptionProfilesManager::GetProfile(const std::string & name) const
    {
        auto profileIt = m_profilesMap.find(name);
        if (profileIt != m_profilesMap.end())
        {
            return profileIt->second.get();
        }
        return nullptr;
    }
}