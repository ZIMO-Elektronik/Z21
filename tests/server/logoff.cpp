#include "base_test.hpp"
#include "utility.hpp"

TEST_F(BaseTest, logoff) {
  EXPECT_EQ(size(_mock.clients()), 0uz);

  // Add first client
  Receive(1,
          "192.168.0.1",
          make_lan_set_broadcastflags_dataset(z21::BroadcastFlags::Default));
  Execute();
  EXPECT_EQ(size(_mock.clients()), 1uz);

  // Add second client
  Receive(1,
          "192.168.0.2",
          make_lan_set_broadcastflags_dataset(z21::BroadcastFlags::Default));
  Execute();
  EXPECT_EQ(size(_mock.clients()), 2uz);

  // Logoff first client
  EXPECT_CALL(_mock, logoff(fd_ip2socket(1, "192.168.0.1")));
  Receive(1, "192.168.0.1", make_lan_logoff_dataset());
  Execute();
  EXPECT_EQ(size(_mock.clients()), 1uz);
}
