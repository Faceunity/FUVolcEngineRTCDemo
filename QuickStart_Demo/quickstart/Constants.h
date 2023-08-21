//
//  Constants.h
//  quickstart
//

#ifndef Constants_h
#define Constants_h

// APPID 使用SDK前需要为自己的应用申请一个AppId，详情参见: https://www.volcengine.com/docs/6348/69865
#define APPID @"64d1f7fd65332a0121ba65b3"

//TOKEN 加入房间的时候需要使用token完成鉴权，详情参见: https://www.volcengine.com/docs/6348/70121
#define TOKEN @"00164d1f7fd65332a0121ba65b3PQCujBsChvjRZAYz22QDAGFiYwQAYWJjZAYAAAAGM9tkAQAGM9tkAgAGM9tkAwAGM9tkBAAGM9tkBQAGM9tkIAClkvfKzJOfyCa5dZ0vaLoIOIxLfNKykBG7GaeXkXblng=="

//INPUT_REGEX SDK 对房间名、用户名的限制是：非空且最大长度不超过128位的数字、大小写字母、@ . _ -
#define INPUT_REGEX @"^[a-zA-Z0-9@._-]{1,128}$"

#endif /* Constants_h */

