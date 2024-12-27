构建数据库用到的命令（我新建了一个数据库）

create database backup;

use backup;

create table users(
     id int primary key auto_increment,
     username varchar(20) unique,
     password char(30)
) ;

直接进行git clone就行

代码说明：

login:负责登录（页面和逻辑）

register1:负责注册（页面和逻辑）

databasemanager:
1. 负责数据库相关的操作
2. 用户注册和用户登录功能全在这里

mainwindow:程序主窗口 （页面和逻辑）

！！！记得运行前在home/qs目录下创建一个our_packup的目录

我想我有必要解释下download这个函数的作用，方便下面工作的开展：
	就是将你选择的还原文件给写到临时文件夹下面（虽然我也没搞懂不直接写到要还原的地方）

创建新的数据库的sql语句如下:
CREATE TABLE history (
    h_id INT PRIMARY KEY AUTO_INCREMENT,  -- 自增长的主键
    time CHAR(30),                        -- 记录时间，最大长度为30字符
    path VARCHAR(100),                    -- 存储备份文件的路径，最大长度为100字符
    size INT,                             -- 文件的大小，以字节为单位
    oldpath VARCHAR(100),                 -- 备份前的路径，最大长度为100字符
    user VARCHAR(20),                     -- 存储用户名，最大长度为20字符
    FOREIGN KEY(user) REFERENCES users(username) -- 外键，关联到users表的username字段
);
