/************************************
 * 调用libxml2库解析xml，提取出电话薄信息
 *
 * @author:Anker  @date:2014/02/09
 * *********************************/

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

#define EXIST_XML_FILE "exist.xml"

#define NEW_BOOK_FILE     "new.xml"
#define NAME_STR_LEN        	50
#define SEX_STR_LEN         	50
#define BIRTH_STR_LEN         	50
#define SCHOOL_STR_LEN         	50
#define PROFESSION_STR_LEN      50
#define ID_STR_LEN              5
//电话通讯录结构体
typedef struct graduate_t {
    char name[NAME_STR_LEN];     	 		//姓名
    char sex[SEX_STR_LEN];     				//性别
    char birth[BIRTH_STR_LEN];       		//生日
    char school[SCHOOL_STR_LEN];  			//学校
	char profession[PROFESSION_STR_LEN]; 	 //专业
}graduate;

//创建graduate节点
static xmlNodePtr create_graduate_node(const graduate *graduate_item)//-->2)
{
    assert(graduate_item);
    
    char id[ID_STR_LEN] = {0};
    xmlNodePtr graduate_node = NULL;
    
    graduate_node = xmlNewNode(NULL, BAD_CAST"graduate");
    if (graduate_node == NULL) {
    fprintf(stderr, "Failed to create new node.\n");
    return NULL;
    }

    xmlNewChild(graduate_node, NULL, BAD_CAST"name", (xmlChar *)graduate_item->name);
    xmlNewChild(graduate_node, NULL, BAD_CAST"sex", (xmlChar *)graduate_item->sex);
    xmlNewChild(graduate_node, NULL, BAD_CAST"birth", (xmlChar *)graduate_item->birth);
	xmlNewChild(graduate_node, NULL, BAD_CAST"school", (xmlChar *)graduate_item->school);
    xmlNewChild(graduate_node, NULL, BAD_CAST"profession", (xmlChar *)graduate_item->profession);

    return graduate_node;
}

//向根节点中添加一个graduate节点
static int add_graduate_node_to_root(xmlNodePtr root_node,graduate *stu)//-->1)set_graduate_item-->2)create_graduate_node
{
	
    xmlNodePtr graduate_node = NULL;
    graduate *graduate_item = NULL;

    //创建一个新的graduate
    graduate_item = (graduate *)malloc(sizeof(graduate));
    if (graduate_item == NULL) {
    fprintf(stderr, "Failed to malloc memory.\n");
    return -1;
    }

    //创建一个graduate节点
    graduate_node = create_graduate_node(stu); 
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
static int create_graduate_books(const char *graduates_book_file,graduate *stu)//-->3)add_graduate_node_to_root
{
    assert(graduates_book_file);
    xmlDocPtr doc = NULL;
    xmlNodePtr root_node = NULL;

    //创建一个xml 文档
    doc = xmlNewDoc(BAD_CAST"1.0");
    if (doc == NULL) {
    fprintf(stderr, "Failed to new doc.\n");
    return -1;
    }

    //创建根节点
    root_node = xmlNewNode(NULL, BAD_CAST"graduates_books");
    if (root_node == NULL) {
    fprintf(stderr, "Failed to new root node.\n");
    goto FAILED;
    }
    //将根节点添加到文档中
    xmlDocSetRootElement(doc, root_node);

    if (add_graduate_node_to_root(root_node,stu) != 0) {
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

static int add_graduate_node(const char *graduates_book_file,graduate *stu)//-->3)add_graduate_node_to_root
{
    assert(graduates_book_file);
    xmlDocPtr doc = NULL;
    xmlNodePtr root_node = NULL;
    xmlNodePtr graduate_node = NULL;
    graduate *graduate_item = NULL;
    xmlKeepBlanksDefault(0);//必须加上，防止程序把元素前后的空白文本符号当作一个node
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
    
    if (add_graduate_node_to_root(root_node,stu) != 0) {
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

static int node_sort(graduate *stu)//获取到首个节点
{
	int i,j,k;
	graduate temp;
	for(i=0;i<4;i++){
		k=i;
		for(j=i+1;j<5;j++){
			if(strcmp(stu[i].birth,stu[j].birth)>0)
				k=j;
		}
		printf("printf :%s\n",stu[k].birth);
		memset(&temp,0,sizeof(graduate));
		memcpy(&temp,stu+i,sizeof(graduate));
		memcpy(stu+i,stu+k,sizeof(graduate));
		memcpy(stu+k,&temp,sizeof(graduate));
	}
}

static int parse_phone_book(const char *file_name,graduate **stu,char* count)
{
	xmlNodePtr element_temp;
	//xmlNodePtr element_cur;
	char *birth_temp,*birth_cur;
    assert(file_name);
	printf("%s %s\n",file_name,__FUNCTION__);
    xmlDocPtr doc;   //xml整个文档的树形结构
    xmlNodePtr cur;  //xml节点
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

	xmlNodePtr element_cur=NULL,CONST;
	xmlChar *key=NULL;
	*count=0;
	char i=0;
	cur = cur->xmlChildrenNode;
	
	CONST = cur;//用于保存最先的节点
	while(cur !=NULL){//用于数个数
		
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"graduate"))) {
			(*count)++;
		}
		cur = cur->next;//每个节点
	}
	*stu =(graduate*)malloc((*count)*sizeof(graduate));
	
	cur = CONST;
	while(cur !=NULL){//和其它一个个人的循环比较
		element_cur = cur->xmlChildrenNode;
		while (element_cur != NULL) {
			//获取name
			if ((!xmlStrcmp(element_cur->name, (const xmlChar *)"name"))) {
				key = xmlNodeListGetString(doc, element_cur->xmlChildrenNode, 1);
				strcpy((*stu)[i].name,(char*)key);
				printf("name: %s\t", key);
				xmlFree(key);
			}
			//获取tel
			if ((!xmlStrcmp(element_cur->name, (const xmlChar *)"sex"))) {
				key = xmlNodeListGetString(doc, element_cur->xmlChildrenNode, 1);
				strcpy((*stu)[i].sex,(char*)key);
				printf("sex: %s\t", key);
				xmlFree(key);
			}
			//获取address
			if ((!xmlStrcmp(element_cur->name, (const xmlChar *)"birth"))) {
				key = xmlNodeListGetString(doc, element_cur->xmlChildrenNode, 1);
				strcpy((*stu)[i].birth,(char*)key);
				printf("sex: %s\t", key);
				xmlFree(key);
			}
			if ((!xmlStrcmp(element_cur->name, (const xmlChar *)"school"))) {
				key = xmlNodeListGetString(doc, element_cur->xmlChildrenNode, 1);
				strcpy((*stu)[i].school,(char*)key);
				printf("school: %s\n", key);
				xmlFree(key);
			}
			if ((!xmlStrcmp(element_cur->name, (const xmlChar *)"profession"))) {
				key = xmlNodeListGetString(doc, element_cur->xmlChildrenNode, 1);
				strcpy((*stu)[i].profession,(char*)key);
				printf("profession: %s\n", key);
				xmlFree(key);
			}
			element_cur = element_cur->next;
		}
		cur = cur->next;i++;
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
    char *xml_file = EXIST_XML_FILE;
	char count=0;
	graduate *student=NULL;
	//printf("student addr:%p\n",&student);
    if (argc == 2) {
    xml_file = argv[1];
    }
    if (parse_phone_book(xml_file,&student,&count) != 0) {
		fprintf(stderr, "Failed to parse phone book.\n");
		return -1;
    }
	node_sort(student);
	
	if (access(NEW_BOOK_FILE, F_OK) == 0) {
		remove(NEW_BOOK_FILE);
	}
	for(int i=0;i<count;i++){
		if (access(NEW_BOOK_FILE, F_OK) == 0) {
		//文件存在，添加一个新的graduate节点
		add_graduate_node(NEW_BOOK_FILE,student+i);
		}
		else {
		//文件不存在，创建一个信息的graduate book
		create_graduate_books(NEW_BOOK_FILE,student+i);
		}
	}
    return 0;
}