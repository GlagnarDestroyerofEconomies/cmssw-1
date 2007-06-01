package confdb.data;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;


/**
 * SoftwareRelease
 * ---------------
 * @author Philipp Schieferdecker
 *
 * Manage software component templates for a release.
 */
public class SoftwareRelease
{
    //
    // member data
    //
    
    /** name of the release */
    private String releaseTag;


    /** list of available EDSource templates */
    private ArrayList<EDSourceTemplate> edsourceTemplates = null;
    
    /** list of available ESSource templates */
    private ArrayList<ESSourceTemplate> essourceTemplates = null;
    
    /** list of available Service templates */
    private ArrayList<ServiceTemplate> serviceTemplates = null;
    
    /** list of available Module templates */
    private ArrayList<ModuleTemplate> moduleTemplates = null;
    
    
    /** hash-map of available EDSource templates BY NAME */
    private HashMap<String,EDSourceTemplate> edsourceTemplatesByName = null;

    /** hash-map of available ESSource templates BY NAME */
    private HashMap<String,ESSourceTemplate> essourceTemplatesByName = null;

    /** hash-map of available Service templates BY NAME */
    private HashMap<String,ServiceTemplate> serviceTemplatesByName = null;

    /** hash-map of available Module templates BY NAME */
    private HashMap<String,ModuleTemplate> moduleTemplatesByName  = null;
    
    
    /** hash-map of available EDSource template names BY DBID */
    private HashMap<Integer,String> edsourceTemplateNamesByDbId = null;

    /** hash-map of available ESSource template names BY DBID */
    private HashMap<Integer,String> essourceTemplateNamesByDbId = null;

    /** hash-map of available Service template names BY DBID */
    private HashMap<Integer,String> serviceTemplateNamesByDbId = null;

    /** hash-map of available Module template names BY DBID */
    private HashMap<Integer,String> moduleTemplateNamesByDbId  = null;
    

    //
    // construction
    //
    
    /** standard constructor */
    public SoftwareRelease()
    {
	releaseTag = new String();
	
	edsourceTemplates    = new ArrayList<EDSourceTemplate>();
	essourceTemplates    = new ArrayList<ESSourceTemplate>();
	serviceTemplates     = new ArrayList<ServiceTemplate>();
	moduleTemplates      = new ArrayList<ModuleTemplate>();
	
	edsourceTemplatesByName = new HashMap<String,EDSourceTemplate>();
	essourceTemplatesByName = new HashMap<String,ESSourceTemplate>();
	serviceTemplatesByName  = new HashMap<String,ServiceTemplate>();
	moduleTemplatesByName   = new HashMap<String,ModuleTemplate>();

	edsourceTemplateNamesByDbId = new HashMap<Integer,String>();
	essourceTemplateNamesByDbId = new HashMap<Integer,String>();
	serviceTemplateNamesByDbId  = new HashMap<Integer,String>();
	moduleTemplateNamesByDbId   = new HashMap<Integer,String>();
     }

    
    //
    // member functions
    //
    
    /** get the release tag */
    public String releaseTag() { return releaseTag; }
    
    /** clear the whole release */
    public void clear(String releaseTag)
    {
	this.releaseTag = releaseTag;

	edsourceTemplates.clear();
	essourceTemplates.clear();
	serviceTemplates.clear();
	moduleTemplates.clear();
	
	edsourceTemplatesByName.clear();
	essourceTemplatesByName.clear();
	serviceTemplatesByName.clear();
	moduleTemplatesByName.clear();

	edsourceTemplateNamesByDbId.clear();
	essourceTemplateNamesByDbId.clear();
	serviceTemplateNamesByDbId.clear();
	moduleTemplateNamesByDbId.clear();
    }
    
    /** clear the instances which might be instantiated */
    public int clearInstances()
    {
	int result = 0;
	for (Template eds : edsourceTemplates) result += eds.removeAllInstances();
	for (Template ess : essourceTemplates) result += ess.removeAllInstances();
	for (Template svc : serviceTemplates)  result += svc.removeAllInstances();
	for (Template mod : moduleTemplates)   result += mod.removeAllInstances();
	return result;
    }

    /** count all instances */
    public int instanceCount()
    {
	int result = 0;
	for (Template eds : edsourceTemplates) result += eds.instanceCount();
	for (Template ess : essourceTemplates) result += ess.instanceCount();
	for (Template svc : serviceTemplates)  result += svc.instanceCount();
	for (Template mod : moduleTemplates)   result += mod.instanceCount();
	return result;
    }
    
    /** EDSource template iterator */
    public Iterator edsourceTemplateIterator()
    {
	return edsourceTemplates.iterator();
    }

    /** ESSource template iterator */
    public Iterator essourceTemplateIterator()
    {
	return essourceTemplates.iterator();
    }

    /** Service template iterator */
    public Iterator serviceTemplateIterator()
    {
	return serviceTemplates.iterator();
    }

    /** Module template iterator */
    public Iterator moduleTemplateIterator()
    {
	return moduleTemplates.iterator();
    }

    /** number of EDSource templates */
    public int edsourceTemplateCount() { return edsourceTemplates.size(); }

    /** number of ESSource templates */
    public int essourceTemplateCount() { return essourceTemplates.size(); }

    /** number of Service templates */
    public int serviceTemplateCount() { return serviceTemplates.size(); }

    /** number of Module templates */
    public int moduleTemplateCount() { return moduleTemplates.size(); }

    /** get EDSource template by name */
    public EDSourceTemplate edsourceTemplate(String name)
    {
	return edsourceTemplatesByName.get(name);
    }
    
    /** get ESSource template by name */
    public ESSourceTemplate essourceTemplate(String name)
    {
	return essourceTemplatesByName.get(name);
    }
    
    /** get Service template by name */
    public ServiceTemplate serviceTemplate(String name)
    {
	return serviceTemplatesByName.get(name);
    }
    
    /** get Module template by name */
    public ModuleTemplate moduleTemplate(String name)
    {
	return moduleTemplatesByName.get(name);
    }
    
    /** get EDSource template name by dbId */
    public String edsourceTemplateName(int dbId)
    {
	return edsourceTemplateNamesByDbId.get(dbId);
    }
    
    /** get ESSource template name by dbId */
    public String essourceTemplateName(int dbId)
    {
	return essourceTemplateNamesByDbId.get(dbId);
    }
    
    /** get Service template name by dbId */
    public String serviceTemplateName(int dbId)
    {
	return serviceTemplateNamesByDbId.get(dbId);
    }
    
    /** get Module template name by dbId */
    public String moduleTemplateName(int dbId)
    {
	return moduleTemplateNamesByDbId.get(dbId);
    }
    
    /** add a template */
    public boolean addTemplate(Template template)
    {
	if (template instanceof EDSourceTemplate) {
	    EDSourceTemplate edsource = (EDSourceTemplate)template;
	    if (!edsourceTemplatesByName.containsKey(edsource.name())) {
		edsourceTemplates.add(edsource);
		edsourceTemplatesByName.put(edsource.name(),edsource);
		edsourceTemplateNamesByDbId.put(edsource.dbSuperId(),edsource.name());
		return true;
	    }
	}
	if (template instanceof ESSourceTemplate) {
	    ESSourceTemplate essource = (ESSourceTemplate)template;
	    if (!essourceTemplatesByName.containsKey(essource.name())) {
		essourceTemplates.add(essource);
		essourceTemplatesByName.put(essource.name(),essource);
		essourceTemplateNamesByDbId.put(essource.dbSuperId(),essource.name());
		return true;
	    }
	}
	else if (template instanceof ServiceTemplate) {
	    ServiceTemplate service = (ServiceTemplate)template;
	    if (!serviceTemplatesByName.containsKey(service.name())) {
		serviceTemplates.add(service);
		serviceTemplatesByName.put(service.name(),service);
		serviceTemplateNamesByDbId.put(service.dbSuperId(),service.name());
		return true;
	    }
	}
	else if (template instanceof ModuleTemplate) {
	    ModuleTemplate module = (ModuleTemplate)template;
	    if (!moduleTemplatesByName.containsKey(module.name())) {
		moduleTemplates.add(module);
		moduleTemplatesByName.put(module.name(),module);
		moduleTemplateNamesByDbId.put(module.dbSuperId(),module.name());
		return true;
	    }
	}
	
	System.out.println("addTemplate("+template.name()+") FAILED!");
	
	return true;
    }
    
}
