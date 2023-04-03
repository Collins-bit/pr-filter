# 脚本文件

部分脚本文件，用于生成测试所需数据

### 目录说明
| 脚本名        | 具体内容            |
|------------|-----------------|
| convert.sh | 生成n组标签对         |
| randmm.sh  | 生成测试数据：指定每行标签数量 |
| getid.sh   | 获取测试文件中指定n组的标签头 |
| getmm.sh   | 统计测试文件中标签对数量    |

### 命令

```asm
$ sh convert.sh 1000 ../testData/inverted.txt ../testData/test.txt
$ sh randmm.sh
$ sh getid.sh 100 ../testData/test.txt
$ sh getmm.sh ../testData/union_MM.txt 1000 ../testData/test.txt
```