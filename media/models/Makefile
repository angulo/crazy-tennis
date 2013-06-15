MODELS := $(patsubst $(DIR_MODELS)%.mesh.xml,$(DIR_MODELS)%.mesh,$(wildcard $(DIR_MODELS)*.mesh.xml))

all: meshes sky.mesh

sky.mesh: 
	@OgreMeshUpgrader sky_base.mesh sky.mesh

meshes: $(MODELS)
	@echo "Done!"

%.mesh:
	@echo "Generating $@..."
	@OgreXMLConverter -q $@.xml 

clean:
	@echo "Cleaning...";
	@rm -rf $(MODELS) *.log
	@ rm -rf sky.mesh
	@echo "Done!"
