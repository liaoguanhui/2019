

#include <stdio.h>
#include <assert.h>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>

#define GRADUATES_BOOK_FILE     "change.xml"
#define DEFAULT_XML_FILE 		"graduates_book.xml"

#define NAME_STR_LEN        	50
#define SEX_STR_LEN         	50
#define BIRTH_STR_LEN         	50
#define SCHOOL_STR_LEN         	50
#define PROFESSION_STR_LEN      50
#define ID_STR_LEN              5
//电话通讯录结构体
typedef struct graduate_t {
	int id;              //编号
    char name[NAME_STR_LEN];     	 		//姓名
    char sex[SEX_STR_LEN];     				//性别
    char birth[BIRTH_STR_LEN];       		//生日
    char school[SCHOOL_STR_LEN];  			//学校
	char profession[PROFESSION_STR_LEN]; 	 //专业
}graduate;

graduate student[5]={{0,"xiao mu","man","1995.03.21","shiyan high school","Electrical engineering"},
						{0,"xiao biao","man","1995.02.21","shiyan high school","medical engineering"},
						{0,"xiao guan","women","1995.05.21","huangpu high school","Electrical engineering"},
						{0,"xiao li","women","1996.03.24","shifan high school","philosophy engineering"},
						{0,"xiao fan","man","1997.03.21","shiyan high school","sea engineering engineering"},
					};

//设置通讯录项
static void set_graduate_item(graduate *graduate_item)//-->1)
{
	static int node_num=0;	//节点计数
    assert(graduate_item);
	graduate_item->id = 1;
    strncpy(graduate_item->name, student[node_num].name,NAME_STR_LEN);
    strncpy(graduate_item->sex, student[node_num].sex,SEX_STR_LEN);
    strncpy(graduate_item->birth, student[node_num].birth,BIRTH_STR_LEN);
	strncpy(graduate_item->school, student[node_num].school,SCHOOL_STR_LEN);
	strncpy(graduate_item->profession, student[node_num].profession,PROFESSION_STR_LEN);
	node_num++;
}

//创建graduate节点
static xmlNodePtr create_graduate_node_1(const graduate *graduate_item)//-->2)
{
    assert(graduate_item);
    
    char id[ID_STR_LEN] = {0};
    xmlNodePtr graduate_node = NULL;
    
    graduate_node = xmlNewNode(NULL, BAD_CAST"graduate");
    if (graduate_node == NULL) {
    fprintf(stderr, "Failed to create new node.\n");
    return NULL;
    }
    //设置属性
    snprintf(id, ID_STR_LEN, "%d", graduate_item->id);
    xmlNewProp(graduate_node, BAD_CAST"id", (xmlChar*)id);

    xmlNewChild(graduate_node, NULL, BAD_CAST"name", (xmlChar *)graduate_item->name);
    xmlNewChild(graduate_node, NULL, BAD_CAST"sex", (xmlChar *)graduate_item->sex);
    xmlNewChild(graduate_node, NULL, BAD_CAST"birth", (xmlChar *)graduate_item->birth);
	xmlNewChild(graduate_node, NULL, BAD_CAST"school", (xmlChar *)graduate_item->school);
    xmlNewChild(graduate_node, NULL, BAD_CAST"profession", (xmlChar *)graduate_item->profession);

    return graduate_node;
}

//创建graduate节点
static xmlNodePtr create_graduate_node_2(const graduate *graduate_item,xmlNodePtr stu,xmlDocPtr doc)//-->2)
{
    assert(graduate_item);
    
    //char id[ID_STR_LEN] = {0};
    xmlNodePtr graduate_node = NULL;
    
    graduate_node = xmlNewNode(NULL, BAD_CAST"graduate");
    if (graduate_node == NULL) {
    fprintf(stderr, "Failed to create new node.\n");
    return NULL;
    }
    //设置属性
    //snprintf(id, ID_STR_LEN, "%d", graduate_item->id);
    //xmlNewProp(graduate_node, BAD_CAST"id", (xmlChar*)id);
	
	xmlChar *key;
    stu = stu->xmlChildrenNode;//节点包含数据和指针，同一级
    while (stu != NULL) {
    //获取name
    if ((!xmlStrcmp(stu->name, (const xmlChar *)"name"))) {
        key = xmlNodeListGetString(doc, stu->xmlChildrenNode, 1);
		xmlNewChild(graduate_node, NULL, BAD_CAST"name", (xmlChar *)key);
        //printf("name: %s\t", key);
        xmlFree(key);
    }
    //获取tel
    if ((!xmlStrcmp(stu->name, (const xmlChar *)"sex"))) {
        key = xmlNodeListGetString(doc, stu->xmlChildrenNode, 1);
		xmlNewChild(graduate_node, NULL, BAD_CAST"sex", (xmlChar *)key);
        //printf("sex: %s\t", key);
        xmlFree(key);
    }
    //获取address
    if ((!xmlStrcmp(stu->name, (const xmlChar *)"birth"))) {
        key = xmlNodeListGetString(doc, stu->xmlChildrenNode, 1);
		xmlNewChild(graduate_node, NULL, BAD_CAST"birth", (xmlChar *)key);
        //printf("birth: %s\n", key);
        xmlFree(key);
    }
	if ((!xmlStrcmp(stu->name, (const xmlChar *)"school"))) {
        key = xmlNodeListGetString(doc, stu->xmlChildrenNode, 1);
		xmlNewChild(graduate_node, NULL, BAD_CAST"school", (xmlChar *)key);
        //printf("school: %s\n", key);
        xmlFree(key);
    }
	if ((!xmlStrcmp(stu->name, (const xmlChar *)"profession"))) {
        key = xmlNodeListGetString(doc, stu->xmlChildrenNode, 1);
		xmlNewChild(graduate_node, NULL, BAD_CAST"profession", (xmlChar *)key);
        //printf("profession: %s\n", key);
        xmlFree(key);
    }
    stu = stu->next;//一个个节点里面元素遍历
    }

    return graduate_node;
}

//向根节点中添加一个graduate节点
static int add_graduate_node_to_root(xmlNodePtr root_node,xmlNodePtr stu,xmlDocPtr doc)//-->1)set_graduate_item-->2)create_graduate_node
{
	xmlNodePtr graduate_node = NULL;
	graduate *graduate_item = NULL;
	//函数复用，恢复原先student全局变量
	//创建一个新的graduate
	graduate_item = (graduate *)malloc(sizeof(graduate));
	if (graduate_item == NULL) {
	fprintf(stderr, "Failed to malloc memory.\n");
	return -1;
	}
 
	//创建一个graduate节点
	if(stu==NULL){
		set_graduate_item(graduate_item);//锁定了填充内容
		graduate_node = create_graduate_node_1(graduate_item); 
	}else{
		graduate_node = create_graduate_node_2(graduate_item,stu,doc); 
	}
	//graduate_node = create_graduate_node_2(graduate_item,stu,doc); 
	if (graduate_node == NULL) {
	fprintf(stderr, "Failed to create graduate node.\n");
	goto FAILED;
	}
	//根节点添加一个子节点
	xmlAddChild(root_node, graduate_node);//新函数：新的开始
	free(graduate_item);

    return 0;
FAILED:
    if (graduate_item){
    free(graduate_item);
    }
    return -1;
}

//创建graduate_books
static int create_graduate_books(const char *graduates_book_file,xmlNodePtr stu)//-->3)add_graduate_node_to_root
{
    assert(graduates_book_file);
	//printf("%s\n",__func__);
    xmlDocPtr doc = NULL;
    xmlNodePtr root_node = NULL;
	xmlKeepBlanksDefault(0);//必须加上，防止程序把元素前后的空白文本符号当作一个node
    //创建一个xml 文档
    doc = xmlNewDoc(BAD_CAST"1.0");
    if (doc == NULL) {
    fprintf(stderr, "Failed to new doc.\n");
    return -1;
    }

    //创建根节点
    root_node = xmlNewNode(NULL, BAD_CAST"graduates_books");//????
    if (root_node == NULL) {
    fprintf(stderr, "Failed to new root node.\n");
    goto FAILED;
    }
    //将根节点添加到文档中
    xmlDocSetRootElement(doc, root_node);

    if (add_graduate_node_to_root(root_node,stu,doc) != 0) {
    fprintf(stderr, "Failed to add a new graduate node.\n");
    goto FAILED;
    }
    //将文档保存到文件中，按照utf-8编码格式保存
    xmlSaveFormatFileEnc(graduates_book_file, doc, "UTF-8", 1);
    //xmlSaveFile("test.xml", doc);
    xmlFreeDoc(doc);

    return 0; 
FAILED:
    if (doc) {
    xmlFreeDoc(doc);
    }

    return -1;
}

static int add_graduate_node(const char *graduates_book_file, xmlNodePtr stu)//-->3)add_graduate_node_to_root
{
    assert(graduates_book_file);
	//printf("%s\n",__func__);
    xmlDocPtr doc = NULL;
    xmlNodePtr root_node = NULL;
    xmlNodePtr graduate_node = NULL;
    graduate *graduate_item = NULL;
    
    doc = xmlParseFile(graduates_book_file);
    if (doc == NULL) {
    fprintf(stderr, "Failed to parser xml file:%s\n", graduates_book_file);
    return -1;
    }

    root_node = xmlDocGetRootElement(doc);
    if (root_node == NULL) {
    fprintf(stderr, "Failed to get root node.\n");
    goto FAILED;
    }
    
    if (add_graduate_node_to_root(root_node,stu,doc) != 0) {
    fprintf(stderr, "Failed to add a new graduate node.\n");
    goto FAILED;
    }
    //将文档保存到文件中，按照utf-8编码格式保存
    xmlSaveFormatFileEnc(graduates_book_file, doc, "UTF-8", 1);
    xmlFreeDoc(doc);

    return 0;
FAILED:
    if (doc) {
    xmlFreeDoc(doc);
    }

    return -1;
}

static int parse_phone_book(const char *file_name)
{
	char *graduates_book_file = GRADUATES_BOOK_FILE;
	xmlNodePtr element_temp;
	xmlNodePtr element_cur;
	xmlChar  *birth_temp,*birth_cur;
    assert(file_name);

	int count=0;
    xmlDocPtr doc;   //xml整个文档的树形结构
    xmlNodePtr cur;  //xml节点
	xmlNodePtr CONST;
	xmlNodePtr cur_element;  //xml节点element
    xmlChar *id;     //phone id
	xmlKeepBlanksDefault(0);//必须加上，防止程序把元素前后的空白文本符号当作一个node
    //获取树形结构
    doc = xmlParseFile(file_name);
    if (doc == NULL) {
    fprintf(stderr, "Failed to parse xml file:%s\n", file_name);
    goto FAILED;
    }

    //获取根节点
    cur = xmlDocGetRootElement(doc);
    if (cur == NULL) {
    fprintf(stderr, "Root is empty.\n");
    goto FAILED;
    }

    if ((xmlStrcmp(cur->name, (const xmlChar *)"graduates_books"))) {
    fprintf(stderr, "The root is not graduates_books.\n");
    goto FAILED;
    }
	cur = cur->xmlChildrenNode;//给cur节点初始化:此时为graduate
	CONST = cur;//用于保存最先的节点
	while(cur !=NULL){//用于数个数
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"graduate"))) {
			count++;
		}
		cur = cur->next;//每个节点
	}
	xmlNodePtr node_temp=NULL;
	for(int i=0;i<count;i++){//5个节点比较出来加载到新的XML文件上去
		cur = CONST;//printf("%d\n",__LINE__);
		//cur = cur->xmlChildrenNode;//给cur节点初始化:此时为graduate
		while(cur !=NULL){
			if ((!xmlStrcmp(cur->name, (const xmlChar *)"graduate"))) {
				
				if (xmlStrcmp(xmlGetProp(cur, "id"), (const xmlChar *)"10"))//通过属性查找未被标记的节点
				{//如果属性值等于“10”就跳过
					node_temp = cur;
					//printf("xmlGetProp:%s\n",xmlGetProp(cur, "id"));
					break;
				}
			}
			cur = cur->next;//每个节点
		}//以上检查无误
		cur = node_temp->next;	//node_temp的下一个节点(可能出现段错误)，以下是一轮的比较
		//printf("%s\n",__func__);
		element_temp = node_temp->xmlChildrenNode;//解析第一个
		while(element_temp !=NULL){//解析出node_temp的birth
			if (!xmlStrcmp(element_temp->name, (const xmlChar *)"birth"))//查找birth的value
			{
				birth_temp = xmlNodeListGetString(doc, element_temp->xmlChildrenNode, 1);
				//printf("birth_temp:%s\n",birth_temp);
				break;
			}
			element_temp = element_temp->next;//节点的每个元素
		}
		while(cur !=NULL){//和其它一个个人的循环比较
			//printf("cur->name:%s\n",cur->name);
			if ((!xmlStrcmp(cur->name, (const xmlChar *)"graduate"))) {
				//printf("第一个id xmlGetProp:%s\n",xmlGetProp(cur, "id"));
				if(xmlStrcmp(xmlGetProp(cur, "id"), (const xmlChar *)"10")){//这里会筛选一些属性
					element_cur = cur->xmlChildrenNode;
					while(element_cur !=NULL){//解析出node_temp->next的birth
						if (!xmlStrcmp(element_cur->name, (const xmlChar *)"birth"))//查找birth的value
						{
							birth_cur = xmlNodeListGetString(doc, element_cur->xmlChildrenNode, 1);
							//printf("第二个的value:%s\n",birth_cur);
							break;
						}
						element_cur = element_cur->next;//节点的每个元素
					}
					if(strcmp(birth_temp,birth_cur) > 0){
						node_temp = cur;
						birth_temp = birth_cur;//用于下一次比较
						
					}
				}
			}	
			cur = cur->next;//每个节点
		}
		xmlSetProp(node_temp, (const xmlChar*)"id", (const xmlChar*)"10");
		//printf("jump out\n");
		/**/
		if (access("change.xml", F_OK) == 0){//文件存在，添加一个新的graduate节点
			//printf("node_temp neicun:%p\n",node_temp);
			add_graduate_node("change.xml",node_temp);
		}else {//文件不存在，创建一个信息的graduate book
			//xmlNodePtr *stu=NULL;
			create_graduate_books("change.xml",node_temp);//添加节点至XML文件
		}
	}
    xmlFreeDoc(doc);
    return 0;
FAILED:
    if (doc) {
    xmlFreeDoc(doc);
    }
    return -1;
}

int main(int argc, char*argv[])//-->parse_phone_book-->parse_phone
{
    char *xml_file = DEFAULT_XML_FILE;

    if (argc == 2) {
    xml_file = argv[1];
    }
	if (access("graduates_book.xml", F_OK) == 0) {
		remove("graduates_book.xml");
	}
	for(int i=0;i<5;i++){
		if (access("graduates_book.xml", F_OK) == 0) {
		//文件存在，添加一个新的graduate节点
		add_graduate_node("graduates_book.xml",NULL);
		}
		else {
		//文件不存在，创建一个信息的graduate book
		create_graduate_books("graduates_book.xml",NULL);
		}
	}//先创建好文件
	
	if (access("change.xml", F_OK) == 0) {
		remove("change.xml");
	}
    if (parse_phone_book(xml_file) != 0) {
    fprintf(stderr, "Failed to parse phone book.\n");
    return -1;
    }

    return 0;
}