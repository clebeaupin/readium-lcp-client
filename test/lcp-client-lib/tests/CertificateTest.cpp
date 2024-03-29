//
//  Created by Artem Brazhnikov on 11/15.
//  Copyright (c) 2014 Readium Foundation and/or its licensees. All rights reserved.
//
//

#include <fstream>
#include <gtest/gtest.h>
#include "TestUtils.h"
#include "TestInfo.h"
#include "CryptoppUtils.h"
#include "Certificate.h"
#include "CertificateRevocationList.h"
#include "EncryptionProfilesManager.h"
#include "public/lcp.h"

namespace lcptest
{
    TEST(CertificateTest, CertificateDistributionPoints)
    {
        lcp::EncryptionProfilesManager profilesManager;
        lcp::IEncryptionProfile * profile = profilesManager.GetProfile("http://readium.org/lcp/profile-1.0");

        ASSERT_NE(profile, nullptr);

        std::unique_ptr<lcp::Certificate> providerCertificate(new lcp::Certificate(TestDistributionPointCert, profile));
        lcp::ICrlDistributionPoints * dp = providerCertificate->DistributionPoints();
        ASSERT_NE(dp, nullptr);

        ASSERT_TRUE(dp->HasCrlDistributionPoints());
        ASSERT_STREQ("http://localhost:8080/test.crl", dp->CrlDistributionPointUrls().front().c_str());
    }

    TEST(CertificateTest, CertificateRevocationList_)
    {
        lcp::EncryptionProfilesManager profilesManager;
        lcp::IEncryptionProfile * profile = profilesManager.GetProfile("http://readium.org/lcp/profile-1.0");

        ASSERT_NE(profile, nullptr);

        std::unique_ptr<lcp::Certificate> providerCertificate(new lcp::Certificate(TestDistributionPointCert, profile));
        lcp::ICrlDistributionPoints * dp = providerCertificate->DistributionPoints();
        ASSERT_NE(dp, nullptr);

        lcp::Buffer rawDecoded = lcp::CryptoppUtils::Base64ToVector(TestCrl);

        lcp::CertificateRevocationList revocation(rawDecoded);

        ASSERT_TRUE(revocation.HasThisUpdateDate());
        ASSERT_TRUE(revocation.HasNextUpdateDate());

        ASSERT_STREQ("20130218T103200Z", revocation.ThisUpdateDate().c_str());
        ASSERT_STREQ("20130218T104200Z", revocation.NextUpdateDate().c_str());

        lcp::StringsSet expected = { "1341767", "1341768", "1341769", "1341770", "1341771" };
        lcp::StringsSet actual = revocation.RevokedSerialNumbers();

        ASSERT_TRUE(ArraysMatch<lcp::StringsSet>(expected, actual));

        ASSERT_TRUE(revocation.SerialNumberRevoked("1341769"));
    }

    TEST(CertificateTest, CertificateParse)
    {
        lcp::EncryptionProfilesManager profilesManager;
        lcp::IEncryptionProfile * profile = profilesManager.GetProfile("http://readium.org/lcp/profile-1.0");
        
        ASSERT_NE(profile, nullptr);

        std::unique_ptr<lcp::Certificate> providerCertificate(new lcp::Certificate(TestCertificate, profile));
        ASSERT_STREQ("14398449449252166955", providerCertificate->SerialNumber().c_str());
        ASSERT_STREQ("20151124T122357Z", providerCertificate->NotBeforeDate().c_str());
        ASSERT_STREQ("20451116T122357Z", providerCertificate->NotAfterDate().c_str());
        ASSERT_EQ(providerCertificate->DistributionPoints(), nullptr);
    }

    TEST(CertificateTest, CertificateVerifyByRoot)
    {
        lcp::EncryptionProfilesManager profilesManager;
        lcp::IEncryptionProfile * profile = profilesManager.GetProfile("http://readium.org/lcp/profile-1.0");

        ASSERT_NE(profile, nullptr);

        std::unique_ptr<lcp::Certificate> rootCertificate(new lcp::Certificate(TestCertificate, profile));
        std::unique_ptr<lcp::Certificate> providerCertificate(new lcp::Certificate(TestCertificate, profile));

        ASSERT_TRUE(providerCertificate->VerifyCertificate(rootCertificate.get()));
    }

    TEST(CertificateTest, CertificateVerifySignature)
    {
        lcp::EncryptionProfilesManager profilesManager;
        lcp::IEncryptionProfile * profile = profilesManager.GetProfile("http://readium.org/lcp/profile-1.0");

        ASSERT_NE(profile, nullptr);

        std::unique_ptr<lcp::Certificate> rootCertificate(new lcp::Certificate(TestCertificate, profile));
        std::unique_ptr<lcp::Certificate> providerCertificate(new lcp::Certificate(TestCertificate, profile));

        ASSERT_TRUE(providerCertificate->VerifyMessage(TestCanonicalJson, TestSignature));
    }

    TEST(CertificateTest, OldTestCertificates)
    {
        std::string canonicalJson = u8"{\"encryption\":{\"content_key\":{\"algorithm\":\"http://www.w3.org/2001/04/xmlenc#aes256-cbc\","
            "\"encrypted_value\":\"/k8RpXqf4E2WEunCp76E8PjhS051NXwAXeTD1ioazYxCRGvHLAck/KQ3cCh5JxDmCK0nRLyAxs1X0aA3z55boQ==\"},\"profile\""
            ":\"http://readium.org/lcp/profile-1.0\",\"user_key\":{\"algorithm\":\"http://www.w3.org/2001/04/xmlenc#sha256\",\"key_check\""
            ":\"jJEjUDipHK3OjGt6kFq7dcOLZuicQFUYwQ+TYkAIWKm6Xv6kpHFhF7LOkUK/Owww\",\"text_hint\":\"Enter your email address\"}},\"id\""
            ":\"ef15e740-697f-11e3-949a-0800200c9a66\",\"issued\":\"2013-11-04T01:08:15+01:00\",\"links\":{\"hint\":{\"href\""
            ":\"http://www.imaginaryebookretailer.com/lcp/hint\",\"type\":\"text/html\"}},\"provider\":\"http://www.imaginaryebookretailer.com\","
            "\"updated\":\"2014-02-21T09:44:17+01:00\",\"user\":{\"id\":\"d9f298a7-7f34-49e7-8aae-4378ecb1d597\"}}";
    }
}