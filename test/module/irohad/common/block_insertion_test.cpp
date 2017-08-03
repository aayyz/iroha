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

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "module/irohad/ametsuchi/ametsuchi_mocks.hpp"
#include "main/raw_block_insertion.hpp"

#include "common/types.hpp"
#include "ametsuchi/block_serializer.hpp"
#include "model/block.hpp"
#include "model/transaction.hpp"
#include "model/commands/add_peer.hpp"
#include "model/commands/transfer_asset.hpp"
#include "common/types.hpp"
#include <iostream>

using ::testing::DefaultValue;
using ::testing::_;
using namespace iroha::main;
using namespace iroha::ametsuchi;
using namespace iroha::model;
using namespace iroha;
using namespace std;

MockMutableStorage *storage_mock = new MockMutableStorage();

std::unique_ptr<MutableStorage> createMutableStorageMock() {
  return unique_ptr<MockMutableStorage>(storage_mock);
}

Transaction getTransaction(uint64_t create_time, std::string address) {
  Transaction transaction;
  transaction.created_ts = create_time;

  AddPeer add_peer;
  add_peer.address = std::move(address);

  TransferAsset transfer_asset;
  transfer_asset.amount = iroha::Amount(69, 42);
  transfer_asset.asset_id = "tugrik";
  transfer_asset.src_account_id = "Tourist";
  transfer_asset.dest_account_id = "Petr";
  transaction.commands = {
      std::make_shared<AddPeer>(add_peer),
      std::make_shared<TransferAsset>(transfer_asset),
  };
  return transaction;
}

Block generateBlock() {
  Block block;
  block.created_ts = 100500;
  block.height = 228;

  block.transactions = {
      getTransaction(322, "ulitsa pushkina, dom kolotushkina"),
      getTransaction(666, "Petushki"),

  };
  return block;
}

TEST(JsonRepr, JsonBlockReprGeneration) {
  cout << "----------| generate block => print |----------" << endl;

  BlockSerializer serializer;
  auto blob = serializer.serialize(generateBlock());
  cout << iroha::bytesToString(blob) << endl;
}

TEST(BlockInsertionTest, BlockInsertionWhenParseBlock) {
  cout << "----------| block => string_repr(block)"
      " => parseBlock() |----------" << endl;

  shared_ptr<MockMutableFactory> factory = make_shared<MockMutableFactory>();
  BlockInserter inserter(factory);
  auto block = generateBlock();
  auto blob = BlockSerializer().serialize(block);
  auto str = iroha::bytesToString(blob);
  auto new_block = inserter.parseBlock(str);
  ASSERT_NE(nonstd::nullopt, new_block);
  ASSERT_EQ(block, new_block.value());
}

TEST(BlockInsertionTest, BlockInsertionWhenApplyToStorage) {
  cout << "----------| block => applyToLedger() |----------" << endl;

  shared_ptr<MockMutableFactory> factory = make_shared<MockMutableFactory>();
  DefaultValue<std::unique_ptr<MutableStorage>>::SetFactory(
      &createMutableStorageMock);

  EXPECT_CALL(*factory, createMutableStorage()).Times(1);
  EXPECT_CALL(*factory, commit_(_)).Times(1);
  EXPECT_CALL(*storage_mock, apply(_, _)).Times(1);

  BlockInserter inserter(factory);
  auto block = generateBlock();
  vector<Block> blocks{block};

  inserter.applyToLedger(blocks);
}