---
title: ePubFile
date: 2024-11-06 16:49:42
tags: ePub
---

## epub crate example

```rust
//open
use epub::doc::EpubDoc;
let doc = EpubDoc::new("path/to/file.epub");
assert!(doc.is_ok());
let doc = doc.unwrap();

//get doc metadata
let title = doc.mdata("title");

```

## epub crate
### doc 
EpubDoc 结构体: 控制epub文档的结构体
```rust
pub struct EpubDoc<R: Read + Seek> {
    // spine ids
    pub spine: Vec<String>,
    // resource id -> (path, mime)
    pub resources: HashMap<String, (PathBuf, String)>,
    // table of content, list of NavPoint in the toc.ncx
    pub toc: Vec<NavPoint>,
    pub metadata: HashMap<String, Vec<String>>,
    // root file base path
    pub root_base: PathBuf,
    // root file full path
    pub root_file: PathBuf,
    pub extra_css: Vec<String>,
    pub unique_identifier: Option<String>,
    pub cover_id: Option<String>,
    /* private fields */
}
```

File -> Cursor -> EpubDoc
```rust
let mut file = File::open("test.epub").unwrap();
let mut buffer = Vec::new();
file.read_to_end(&mut buffer).unwrap();

let cursor = Cursor::new(buffer);

let doc = EpubDoc::from_reader(cursor);
assert!(doc.is_ok());
let doc = doc.unwrap();
```

## ePub文件结构
ePub文件相当于电子书文件内容和提供索引、书本信息的文件的压缩包。主要包括以下三个部分：META-INF、OEBPS、mimetype。ePub解析包括对这三部分的解析。mimetype指定MIME媒体类型；META—INF存放container.xml，定义.opf文件的路径及media-type；OEBPS存放电子书内容，包括章节内容（xhtml文件），书本信息（content.opf文件），图片内容，目录索引（toc.ncx）等等。

## ePub书籍解析设计
1. 主要元数据（位于content.opf）
- metadata标签
- manifest标签
- spine标签
- guide标签
- tour标签
- ncx目录文件
- css样式表
2. 解析流程
解析ePub书籍主要流程：章节目录列表的建立，样式表的读取、章节文件的读取。对于每块元数据的读取提供api。
首先读取META-INF/container.xml文件得到content.opf文件路径，解析.opf文件进行epub书籍的元数据的创建和初始化；
解析toc目录文件ncx，建立树形目录结构；
解析.css样式表文件，建议与xhtml章节文件的映射关系；
解析章节内容，基于spine顺序的xhtml文件的集合，顺序解析各个文件，将文件进行指定格式的解析储存，方便前端进行格式解析渲染

## ePub模块api设计
* ePub文件的上传和元数据的解析(FileService)
```md
### 上传书本，支持格式：epub [POST /api/file/v1/upload]
+ Request (multipart/form-data)
    + Attributes
        + file: (file, required) - 文件
+ Response 200 (application/json)
    + Attributes (BaseResponse[string])
```

