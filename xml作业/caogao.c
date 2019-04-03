

1.创建XML文件
2.xmlNodePtr node_temp=NULL;
2.for(int i=0;i<5;i++){//5个节点比较出来加载到新的XML文件上去
	cur = root->xmlChildrenNode;//给cur节点初始化
	while(cur !=NULL){
		if (xmlStrcmp(xmlGetProp(cur, "id"), (const xmlChar *)"10"))//查找未被标记的节点
		{//如果属性值等于“10”就跳过
			node_temp = cur;
			break;
		}
		cur = cur->next;
	}
	cur = node_temp->next;	//node_temp的下一个节点，以下是一轮的比较
	if (!xmlStrcmp(node_temp->name, BAD_CAST("birth")))//查找birth的value
	{
		birth_temp = ((char*)XML_GET_CONTENT(node_temp->xmlChildrenNode)));
	}
	
	while(cur !=NULL){
		if (!xmlStrcmp(cur->name, BAD_CAST("birth"))&&(xmlStrcmp(xmlGetProp(cur, "id"), (const xmlChar *)"10")))
		{//如果属性值等于“10”就跳过
			if(strcmp(birth_temp,(char*)XML_GET_CONTENT(nptr->xmlChildrenNode)) > 0){
				node_temp = cur;
				if (!xmlStrcmp(node_temp->name, BAD_CAST("birth"))){//查找birth的value
					birth_temp = ((char*)XML_GET_CONTENT(node_temp->xmlChildrenNode)));
				}
			}
			//break;
		}
		cur = cur->next;
	}
	if (access(graduates_book_file, F_OK) == 0) {
		//文件存在，添加一个新的graduate节点
		add_graduate_node(graduates_book_file,node_temp);
		//create_graduate_books(graduates_book_file,node_temp);//添加节点至XML文件
		}
		else {
		//文件不存在，创建一个信息的graduate book
		//create_graduate_books(graduates_book_file,stu);
		create_graduate_books(graduates_book_file,node_temp);//添加节点至XML文件
	}
	//create_graduate_books(graduates_book_file,node_temp);//添加节点至XML文件
	xmlSetProp(node_temp, (cosnt xmlChar*)"id", (const xmlChar*)"10");
}