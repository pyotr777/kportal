#include "../stdafx.h"
#include "TarUtil.hpp"

#define REPOSITORIES_FILE "repositories"

ResponseCode UpdateRepoTagOfTarball(std::string pathTarball, std::string repo, std::string tag, std::string &idImage) {
  TAR *tar = NULL;
  int ret = 0;
  int th_found = 0;
  int exitcode = 0;
//  char tmp_filepath[] = P_tmpdir "/libtar-tmpfile-XXXXXX";
  //gen path to upload image
  char* tmp_imagepath = tempnam (FileUtils::GetPathDir(pathTarball).c_str(), "imageDirTmp_");
  std::string pathTmpDir = tmp_imagepath;
  free(tmp_imagepath);
  std::cout << "path tmp dir: " << pathTmpDir << std::endl;

  int tmp_fd = -1;

  ret = tar_open(&tar, (char*)pathTarball.c_str(), NULL, O_RDONLY, 0, 0);
  if (ret != 0) {
	fprintf(stdout, "Fail to open file: %s\n", pathTarball.c_str());
//	return FILE_ACTION_ERROR;
	exitcode = 2;
  }
	if (exitcode == 0) {
		if (tar_extract_all(tar, (char*)pathTmpDir.c_str()) != 0) {
            fprintf(stderr, "Fail to extract file: %s\n", pathTarball.c_str());
			exitcode = 2;
		}
	}

	if (exitcode == 0) {
		ret = tar_close(tar);
		if (ret != 0) {
			perror("Failed to close tar file.");
			exitcode = 2;
		}
		tar = NULL;
	}

  //Modify repository file
  if (exitcode == 0) {
	char buf[BUFSIZ];
	ssize_t n_buf = 0;
	FILE *tmpfile = NULL;

	std::ifstream in((pathTmpDir + "/" + REPOSITORIES_FILE).c_str(), std::ios::binary);
	std::string info((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

	JSONNode n;
	try
	{
		n = libjson::parse(info);
	} catch(const std::invalid_argument& e) {
		std::cerr << "Invalid argument: " << e.what() << '\n';
//		return FILE_ACTION_ERROR;
		exitcode = 1;
	}
	if(exitcode == 0){
		JSONNode::const_iterator i = n.begin();
		//if (i != n.end() && i -> name() == TAG_SERVICE_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
		if (i != n.end() && i -> type() == JSON_NODE){
			JSONNode tag_id_node = i->as_node();
			i = tag_id_node.begin();
			if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE){
				idImage = i->as_string();
			}else{
				std::cout << "Tarball format error.\n";
		//		return FILE_ACTION_ERROR;
				exitcode = 1;
			}
		}
	}else{
		std::cout << "Tarball format error.\n";
//			return FILE_ACTION_ERROR;
		exitcode = 1;
		}
	}

	if(exitcode == 0){
		JSONNode newRepoNode(JSON_NODE);
		newRepoNode.set_name(repo);
		newRepoNode.push_back(JSONNode(tag, idImage));

		JSONNode newNode;
		newNode.push_back(newRepoNode);
		std::string content = newNode.write();

		FILE * pFile;

		if (exitcode == 0) {
			pFile = fopen ((pathTmpDir + "/" + REPOSITORIES_FILE).c_str() , "w");
			if (pFile == NULL) {
			   printf("Error opening file %s\n", (pathTmpDir + "/" + REPOSITORIES_FILE).c_str());
		//	   return FILE_ACTION_ERROR;
			   exitcode = 1;
			}
		}

		if (exitcode == 0) {
			fwrite (content.c_str() , sizeof(char), content.size(), pFile);
			fclose (pFile);
			printf("content tmp file: %s\n", content.c_str());
		}
	}

	if (exitcode == 0) {
		remove (pathTarball.c_str());
		ret = tar_open(&tar, (char*)pathTarball.c_str(), NULL, O_WRONLY | O_CREAT, 0600, 0);
		if (ret != 0) {
		  fprintf(stderr, "Fail to open file: %s\n", pathTarball.c_str());
		//	  return FILE_ACTION_ERROR;
		  exitcode = 2;
		}

		if (exitcode == 0) {
			if (tar_append_tree(tar, (char*)pathTmpDir.c_str(), "") != 0) {
			  fprintf(stderr, "Fail to compress file: %s\n", pathTarball.c_str());
		//		  return FILE_ACTION_ERROR;
			  exitcode = 2;
			}
		}

		if (exitcode == 0) {
			ret = tar_close(tar);
			if (ret != 0) {
				perror("Failed to close tar file.");
				exitcode = 2;
			} else {
				tar = NULL;
			}
		}
	}
	std::cout << "delete_folder_tree: " << pathTmpDir.c_str() << std::endl;
	if (FileUtils::delete_folder_tree(pathTmpDir.c_str())) {
		fprintf(stderr, "Fail to delete temp dir: %s\n", pathTmpDir.c_str());
	}

	if (exitcode == 0) {
		return FILE_ACTION_SUCCESS;
	} else if (exitcode == 1) {
		return FILE_ACTION_ERROR;
	} else {
		return DATA_ERROR;
	}
}
