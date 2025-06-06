#include "UIUtils.h"

// TODO: refactor to move this dependency to common code
#include "../Debug.h"
#include "../f4vr/F4VROffsets.h"

namespace vrui {
	/**
	 * Get struct with useful NiNodes references related to player.
	 */
	f4vr::PlayerNodes* getPlayerNodes() {
		return reinterpret_cast<f4vr::PlayerNodes*>(reinterpret_cast<char*>(*g_player) + 0x6E0);
	}

	/**
	 * Update the node flags to show/hide it.
	 */
	void setNodeVisibility(NiNode* node, const bool visible, const float originalScale) {
		node->m_localTransform.scale = visible ? originalScale : 0;

		// TODO: try to understand why it's not working for our nifs.
		//if (visible)
		//	node->flags &= 0xfffffffffffffffe; // show
		//else
		//	node->flags |= 0x1; // hide
		//NiAVObject::NiUpdateData* ud = nullptr;
		//node->UpdateWorldData(ud);
	}

	/**
	 * Get a NiNode that can be used in game UI for the given .nif file.
	 * Why is just loading not enough?
	 */
	NiNode* getClonedNiNodeForNifFile(const std::string& path) {
		auto& normPath = path._Starts_with("Data") ? path : "Data/Meshes/" + path;
		const NiNode* nifNode = loadNifFromFile(normPath.c_str());
		f4vr::NiCloneProcess proc;
		proc.unk18 = f4vr::cloneAddr1;
		proc.unk48 = f4vr::cloneAddr2;
		const auto uiNode = f4vr::cloneNode(nifNode, &proc);
		uiNode->m_name = BSFixedString(path.c_str());
		return uiNode;
	}

	/**
	 * Load .nif file from the filesystem and return the root node.
	 */
	NiNode* loadNifFromFile(const char* path) {
		uint64_t flags[2] = {0x0, 0xed};
		uint64_t mem = 0;
		int ret = f4vr::loadNif((uint64_t)path, (uint64_t)&mem, (uint64_t)&flags);
		return reinterpret_cast<NiNode*>(mem);
	}

	/**
	 * Find node by name in the given subtree of the given root node.
	 */
	NiNode* findNode(const char* nodeName, NiNode* node) {
		if (!node || !node->m_name) {
			return nullptr;
		}

		if (_stricmp(nodeName, node->m_name.c_str()) == 0) {
			return node;
		}

		if (node->GetAsNiNode()) {
			for (auto i = 0; i < node->m_children.m_emptyRunStart; ++i) {
				if (const auto nextNode = node->m_children.m_data[i]) {
					if (const auto ret = findNode(nodeName, dynamic_cast<NiNode*>(nextNode))) {
						return ret;
					}
				}
			}
		}
		return nullptr;
	}

	static void getNodeWidthHeight(NiNode* node) {
		const auto shape = node->GetAsBSTriShape();
		auto bla = shape->geometryData->vertexData->vertexBlock[0];
	}
}
