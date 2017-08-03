/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef IROHA_CONSENSUS_INIT_HPP
#define IROHA_CONSENSUS_INIT_HPP

#include <gmock/gmock.h>
#include <memory>
#include <string>
#include <vector>
#include "consensus/yac/yac.hpp"
#include "consensus/yac/messages.hpp"
#include "consensus/yac/impl/yac_gate_impl.hpp"
#include "consensus/yac/impl/network_impl.hpp"
#include <consensus/yac/impl/timer_impl.hpp>
#include <consensus/yac/impl/peer_orderer_impl.hpp>
#include <consensus/yac/impl/yac_hash_provider_impl.hpp>

namespace iroha {
  namespace consensus {
    namespace yac {

      class MockYacCryptoProvider : public YacCryptoProvider {
       public:
        MOCK_METHOD1(verify, bool(CommitMessage));
        MOCK_METHOD1(verify, bool(RejectMessage));
        MOCK_METHOD1(verify, bool(VoteMessage));

        VoteMessage getVote(YacHash hash) override {
          VoteMessage vote;
          vote.hash = hash;
          return vote;
        }

        MockYacCryptoProvider() = default;

        MockYacCryptoProvider(const MockYacCryptoProvider &) {}

        MockYacCryptoProvider &operator=(const MockYacCryptoProvider &) {
          return *this;
        }
      };

      class YacInit {
       public:
        auto initConsensusGate(std::string network_address,
                               std::shared_ptr<uvw::Loop> loop,
                               std::unique_ptr<YacPeerOrderer> peer_orderer) {
          auto yac = createYac(network_address,
                               loop,
                               peer_orderer->getInitialOrdering().value());

          auto hash_provider = createHashProvider();
          return std::make_shared<YacGateImpl>(std::unique_ptr<HashGate>(yac.release()),
                                               peer_orderer,
                                               hash_provider);
        }

        std::shared_ptr<NetworkImpl> consensus_network;
       private:
        std::unique_ptr<consensus::yac::Yac> createYac(std::string network_address,
                                                       std::shared_ptr<uvw::Loop> loop,
                                                       ClusterOrdering initial_order) {
          uint64_t delay_seconds = 5;

          return Yac::create(YacVoteStorage(),
                             createNetwork(network_address,
                                           initial_order.getPeers()),
                             createCryptoProvider(),
                             createTimer(loop),
                             initial_order,
                             delay_seconds * 1000);

        }

        // ----------| Yac dependencies |----------

        auto createNetwork(std::string network_address,
                           std::vector<model::Peer> initial_peers) {
          // todo set as field
          return std::make_shared<NetworkImpl>(network_address, initial_peers);
        }

        auto createCryptoProvider() {
          std::shared_ptr<MockYacCryptoProvider>
              crypto = std::make_shared<MockYacCryptoProvider>();

          EXPECT_CALL(crypto, verify(testing::An<CommitMessage>()))
              .WillRepeatedly(testing::Return(true));

          EXPECT_CALL(crypto, verify(testing::An<RejectMessage>()))
              .WillRepeatedly(testing::Return(true));

          EXPECT_CALL(crypto, verify(testing::An<VoteMessage>()))
              .WillRepeatedly(testing::Return(true));
          return crypto;
        }

        auto createTimer(std::shared_ptr<uvw::Loop> loop) {
          return std::make_shared<TimerImpl>(loop);
        }

        auto createHashProvider() {
          return std::make_shared<YacHashProviderImpl>();
        }
      };
    } // namespace yac
  } // namespace consensus
} // namespace iroha

#endif //IROHA_CONSENSUS_INIT_HPP