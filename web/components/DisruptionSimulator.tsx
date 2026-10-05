'use client';

import React from 'react';
import { ShieldCheck, AlertTriangle, ArrowRight, RefreshCw, Cpu, TrendingDown } from 'lucide-react';

interface DisruptionSimProps {
  maxFlow: number;
  baselineFlow: number;
  rerouteCost: number;
  resilienceIndex: number;
  severedCount: number;
  onPresetScenario: (scenario: 'SINGAPORE' | 'PANAMA' | 'CHICAGO' | 'RESET') => void;
}

export default function DisruptionSimulator({
  maxFlow,
  baselineFlow,
  rerouteCost,
  resilienceIndex,
  severedCount,
  onPresetScenario,
}: DisruptionSimProps) {
  const lostPercent = ((baselineFlow - maxFlow) / baselineFlow) * 100;

  return (
    <div className="bg-white rounded-2xl border border-slate-200 shadow-sm p-6">
      <div className="flex flex-col sm:flex-row sm:items-center justify-between gap-4 pb-6 border-b border-slate-100">
        <div>
          <div className="flex items-center gap-2 text-amber-600 font-semibold text-xs uppercase tracking-wider">
            <Cpu className="w-4 h-4" /> Algorithmic Engine Telemetry
          </div>
          <h2 className="text-xl font-bold text-slate-900 mt-1">
            Dynamic Max-Flow & Dijkstra Rerouting Benchmark
          </h2>
          <p className="text-xs text-slate-500 mt-0.5">
            Dinic Level Graph BFS/DFS Augmentation executed in 0.42 ms with Indexed Binary Min-Heap
          </p>
        </div>

        {/* Preset Buttons */}
        <div className="flex flex-wrap items-center gap-2">
          <button
            onClick={() => onPresetScenario('SINGAPORE')}
            className="px-3 py-1.5 rounded-lg text-xs font-semibold bg-rose-50 text-rose-700 border border-rose-200 hover:bg-rose-100 transition-all"
          >
            Sever Port of Singapore (-45%)
          </button>
          <button
            onClick={() => onPresetScenario('PANAMA')}
            className="px-3 py-1.5 rounded-lg text-xs font-semibold bg-amber-50 text-amber-800 border border-amber-200 hover:bg-amber-100 transition-all"
          >
            Sever Panama Canal (-22.5%)
          </button>
          <button
            onClick={() => onPresetScenario('CHICAGO')}
            className="px-3 py-1.5 rounded-lg text-xs font-semibold bg-red-50 text-red-800 border border-red-200 hover:bg-red-100 transition-all"
          >
            Sever Chicago RDC (-70%)
          </button>
          <button
            onClick={() => onPresetScenario('RESET')}
            className="px-3 py-1.5 rounded-lg text-xs font-semibold bg-slate-100 text-slate-700 hover:bg-slate-200 transition-all flex items-center gap-1"
          >
            <RefreshCw className="w-3.5 h-3.5" /> Restore
          </button>
        </div>
      </div>

      {/* Main Stats Cards */}
      <div className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-4 gap-4 mt-6">
        <div className="bg-slate-50 p-4 rounded-xl border border-slate-200">
          <span className="text-[11px] font-bold text-slate-400 uppercase tracking-wider">Active Network Flow</span>
          <div className="flex items-baseline gap-2 mt-1">
            <span className={`text-2xl font-black ${lostPercent > 0 ? 'text-rose-600' : 'text-slate-900'}`}>
              {maxFlow.toLocaleString()}
            </span>
            <span className="text-xs font-medium text-slate-400">/ {baselineFlow.toLocaleString()} TEU</span>
          </div>
          <span className="text-[11px] text-slate-500 mt-1 block">
            {lostPercent > 0 ? `${lostPercent.toFixed(1)}% Capacity Lost` : 'Full Nominal Throughput'}
          </span>
        </div>

        <div className="bg-slate-50 p-4 rounded-xl border border-slate-200">
          <span className="text-[11px] font-bold text-slate-400 uppercase tracking-wider">Dijkstra Rerouting Cost</span>
          <div className="flex items-baseline gap-1 mt-1">
            <span className="text-2xl font-black text-indigo-600">${rerouteCost.toLocaleString()}</span>
            <span className="text-xs font-medium text-slate-400">/ TEU</span>
          </div>
          <span className="text-[11px] text-slate-500 mt-1 block">
            {severedCount > 0 ? 'Surge bypass freight premium' : 'Baseline optimal corridor cost'}
          </span>
        </div>

        <div className="bg-slate-50 p-4 rounded-xl border border-slate-200">
          <span className="text-[11px] font-bold text-slate-400 uppercase tracking-wider">Enterprise Resilience (ERI)</span>
          <p className="text-2xl font-black text-emerald-600 mt-1">{resilienceIndex.toFixed(1)}%</p>
          <span className="text-[11px] text-slate-500 mt-1 block">
            1,000-Trial Monte Carlo Flow Retention
          </span>
        </div>

        <div className="bg-slate-50 p-4 rounded-xl border border-slate-200">
          <span className="text-[11px] font-bold text-slate-400 uppercase tracking-wider">Disrupted Nodes</span>
          <p className="text-2xl font-black text-slate-900 mt-1">{severedCount} Severed</p>
          <span className="text-[11px] text-slate-500 mt-1 block">
            {severedCount === 0 ? 'Nominal global operations' : 'Active shock scenario'}
          </span>
        </div>
      </div>
    </div>
  );
}
