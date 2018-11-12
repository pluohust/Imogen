// https://github.com/CedricGuillemet/Imogen
//
// The MIT License(MIT)
// 
// Copyright(c) 2018 Cedric Guillemet
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#pragma once
#include "Evaluation.h"

struct EvaluationContext
{
	EvaluationContext(Evaluation& evaluation, bool synchronousEvaluation, int defaultWidth, int defaultHeight);
	~EvaluationContext();

	void RunAll();
	void RunForward(size_t nodeIndex);
	void RunBackward(size_t nodeIndex);
	void RunSingle(size_t nodeIndex, int width, int height, EvaluationInfo& evaluationInfo);
	void RunDirty();

	unsigned int GetEvaluationTexture(size_t target);
	RenderTarget *GetRenderTarget(size_t target)
	{ 
		if (target >= mStageTarget.size())
			return NULL;
		return mStageTarget[target]; 
	}

	FFMPEGCodec::Encoder *GetEncoder(const std::string &filename, int width, int height);
	bool IsSynchronous() const { return mbSynchronousEvaluation; }
	void SetTargetDirty(size_t target, bool onlyChild = false);

protected:
	Evaluation& mEvaluation;

	void EvaluateGLSL(const EvaluationStage& evaluationStage, size_t index, EvaluationInfo& evaluationInfo);
	void EvaluateC(const EvaluationStage& evaluationStage, size_t index, EvaluationInfo& evaluationInfo);
	void RunNodeList(const std::vector<size_t>& nodesToEvaluate);

	void RecurseBackward(size_t target, std::vector<size_t>& usedNodes);
	void RecurseForward(size_t base, size_t parent, std::vector<size_t>& usedNodes);

	void AllocRenderTargetsForEditingPreview();
	void AllocRenderTargetsForBaking();

	std::vector<RenderTarget*> mStageTarget; // 1 per stage
	std::vector<RenderTarget*> mAllocatedTargets; // allocated RT, might be present multiple times in mStageTarget
	std::map<std::string, FFMPEGCodec::Encoder*> mWriteStreams;
	std::vector<bool> mDirty;
	int mDefaultWidth;
	int mDefaultHeight;
	bool mbSynchronousEvaluation;
};

extern EvaluationContext *gCurrentContext;
